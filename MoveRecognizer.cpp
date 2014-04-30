#include "MoveRecognizer.h"

using namespace cv;


MoveRecognizer::MoveType MoveRecognizer::recognize(vector<ChangeInfo>& changes, bool isWhiteMove, vector<Point2i>& moveCoords) {
	int white, black, empty = EMPTY_FIELD;
	moveCoords.clear();
	this->moveCoords = &moveCoords;

	if (changes.size() == 0)
		return NO_MOVE;
	
	// ka¿dy z wykrywanych ruchów sk³ada sie z 2,3 lub 4 zmian pól
	if (changes.size() < 2 || changes.size() > 4)
		return INCORRECT;

	// ustalenie indeksu tablicy zmian dla koloru wykonuj¹cego ruch
	if (isWhiteMove) {
		white = MOVED_FIGURE_COLOR;
		black = SECOND_FIGURE_COLOR;
		movedFigureColor = 'w';
		secondFigureColor = 'b';
	}
	else {
		white = SECOND_FIGURE_COLOR;
		black = MOVED_FIGURE_COLOR;
		movedFigureColor = 'b';
		secondFigureColor = 'w';
	}
	emptyField = 0;

	// id: 0 - kolor figury wykonuj¹cej ruch, 1 - kolor figury przeciwnej,
	//     2 - pole puste
	for (int i=0; i<3; i++)
		orderedChanges[i].clear();

	// zape³nienie wektorów zmian stanów pól dla poszczególnych kolorów figur
	for (int i=0; i<changes.size(); i++) {
		switch (changes[i].colorLastState) {
			case 'w':
				orderedChanges[white].push_back(changes[i]); break;
			case 'b':
				orderedChanges[black].push_back(changes[i]); break;
			case 0:
				orderedChanges[empty].push_back(changes[i]); break;
			default:
				return INCORRECT;
		}
	}

	/*
		Jeœli za kolor figury wykonuj¹cej ruch przyj¹æ A, za przeciwny kolor: B,
		a za puste pole 0, to poszczególne mo¿liwoœci ruchu przedstawiaj¹ siê
		jako zbiory zmian stanów pól (ponumerowano, ale kolejnoœæ dowolna):

		* przesuniêcie figury na wolne pole:
		1. A -> 0 (pole z figur¹ koloru A staje siê wolne)
		2. 0 -> A (inne pole, puste, w nowym stanie jest zajmowane przez figurê)

		* bicie (figura koloru A bije figurê koloru B):
		1. A -> 0
		2. B -> A (figura koloru A staje na polu zajmowanym wczeœniej przez B)

		* roszada (dwa pola staj¹ siê puste, a dwa inne, puste, staj¹ siê zajête)
		1. A -> 0
		2. 0 -> A
		3. A -> 0
		4. 0 -> A
		
		* bicie w przelocie
		1. A -> 0
		2. 0 -> A
		3. B -> 0

		W przypadku roszady i bicia w przelocie sprawdzane te¿ s¹ dodatkowe
		warunki (po³o¿enie figur na szachownicy, oddalenie od siebie itp.).
		Pozwala to lepiej odró¿niæ nieprawid³owy ruch lub Ÿle wykryte figury.

		Jest to jednak bardzo przybli¿one i s³u¿y tylko i wy³¹cznie odró¿nieniu
		nieprawid³owo wykrytych figur na szachownicy, a nie w³aœciwej detekcji
		konkretnych figur na szachownicy (tym zajmuje siê modu³ logiki gry).
	*/

	// W ka¿dym rozpoznawanym tutaj ruchu wystêpuje co najmniej jedna zmiana
	// A -> 0, tzn. pole zajmowane przez figurê koloru bij¹cego staje siê wolne
	// (bo figura wykonuje ruch, zatem jej poprzednie pole staje siê puste).
	if (orderedChanges[MOVED_FIGURE_COLOR].size() == 0)
		return INCORRECT;

	else if (changes.size() == 2) {
		if (orderedChanges[MOVED_FIGURE_COLOR].size() != 1)
			return INCORRECT;
		if (orderedChanges[EMPTY_FIELD].size() == 1)
			return isMove() ? MOVE : INCORRECT;
		else
			return isCapturing() ? CAPTURING : INCORRECT;
	}

	else if (changes.size() == 3) {
		return isEnPassant() ? EN_PASSANT : INCORRECT;
	}

	else { // changes.size() == 4
		return isCastling() ? CASTLING : INCORRECT;
	}
}

bool MoveRecognizer::isMove() {
	// figura koloru, który wykonywa³ ruch, przesunê³a siê na wolne pole

	if (orderedChanges[MOVED_FIGURE_COLOR].size() != 1 ||
		orderedChanges[SECOND_FIGURE_COLOR].size() != 0 ||
		orderedChanges[EMPTY_FIELD].size() != 1)
			return false;
	if (orderedChanges[MOVED_FIGURE_COLOR][0].colorNewState != emptyField ||
		orderedChanges[EMPTY_FIELD][0].colorNewState != movedFigureColor)
			return false;

	moveCoords->push_back(orderedChanges[MOVED_FIGURE_COLOR][0].coords);
	moveCoords->push_back(orderedChanges[EMPTY_FIELD][0].coords);

	printf("MoveRecognizer: rozpoznano przesuniecie figury na wolne pole: %c%d => %c%d\n",
		'A'+(*moveCoords)[0].x, 1+(*moveCoords)[0].y, 'A'+(*moveCoords)[1].x, 1+(*moveCoords)[1].y);
	return true;
}

bool MoveRecognizer::isCapturing() {
	// bicie: figura koloru, który wykonywa³ ruch, przesunê³a siê na pole
	// zajmowane wczeœniej przez figurê koloru przeciwnego

	if (orderedChanges[MOVED_FIGURE_COLOR].size() != 1 ||
		orderedChanges[SECOND_FIGURE_COLOR].size() != 1 ||
		orderedChanges[EMPTY_FIELD].size() != 0)
			return false;
	if (orderedChanges[MOVED_FIGURE_COLOR][0].colorNewState != emptyField ||
		orderedChanges[SECOND_FIGURE_COLOR][0].colorNewState != movedFigureColor)
			return false;

	moveCoords->push_back(orderedChanges[MOVED_FIGURE_COLOR][0].coords);
	moveCoords->push_back(orderedChanges[SECOND_FIGURE_COLOR][0].coords);

	printf("MoveRecognizer: rozpoznano bicie: %c%d => %c%d\n",
		'A'+(*moveCoords)[0].x, 1+(*moveCoords)[0].y, 'A'+(*moveCoords)[1].x, 1+(*moveCoords)[1].y);
	return true;
}

bool MoveRecognizer::isCastling() {
	// roszada: figury ze skrajnej linii szachownicy przesuwaj¹ siê
	// na wolne pola po³o¿one miêdzy nimi

	if (orderedChanges[MOVED_FIGURE_COLOR].size() != 2 ||
		orderedChanges[SECOND_FIGURE_COLOR].size() != 0 ||
		orderedChanges[EMPTY_FIELD].size() != 2)
			return false;
	if (orderedChanges[MOVED_FIGURE_COLOR][0].colorNewState != emptyField ||
		orderedChanges[MOVED_FIGURE_COLOR][1].colorNewState != emptyField ||
		orderedChanges[EMPTY_FIELD][0].colorNewState != movedFigureColor ||
		orderedChanges[EMPTY_FIELD][1].colorNewState != movedFigureColor)
			return false;

	// wszystkie zmienione pola znajduj¹ siê w jednej linii (wierszu):
	// pierwszej lub ostatniej (we wspó³rzêdnych szachownicy)
	int line = orderedChanges[MOVED_FIGURE_COLOR][0].coords.y;
	if (!(line == 0 || line == 7) ||
		orderedChanges[MOVED_FIGURE_COLOR][1].coords.y != line ||
		orderedChanges[EMPTY_FIELD][0].coords.y != line ||
		orderedChanges[EMPTY_FIELD][1].coords.y != line)
			return false;

	// pierwszy ruch: król, drugi ruch: wie¿a
	int rookFrom = (orderedChanges[MOVED_FIGURE_COLOR][0].coords.x == 0 ||
				    orderedChanges[MOVED_FIGURE_COLOR][0].coords.x == 7) ?
					0 : 1;
	int kingFrom = !rookFrom;
	int rookTo = abs(orderedChanges[EMPTY_FIELD][0].coords.x - orderedChanges[MOVED_FIGURE_COLOR][kingFrom].coords.x) == 1 ?
				  0 : 1;
	int kingTo = !rookTo;

	moveCoords->push_back(orderedChanges[MOVED_FIGURE_COLOR][kingFrom].coords);
	moveCoords->push_back(orderedChanges[EMPTY_FIELD][kingTo].coords);
	moveCoords->push_back(orderedChanges[MOVED_FIGURE_COLOR][rookFrom].coords);
	moveCoords->push_back(orderedChanges[EMPTY_FIELD][rookTo].coords);

	
	printf("MoveRecognizer: rozpoznano roszade: krol %c%d => %c%d, wieza: %c%d => %c%d\n",
		'A'+(*moveCoords)[0].x, 1+(*moveCoords)[0].y, 'A'+(*moveCoords)[1].x, 1+(*moveCoords)[1].y,
		'A'+(*moveCoords)[2].x, 1+(*moveCoords)[2].y, 'A'+(*moveCoords)[3].x, 1+(*moveCoords)[3].y);
	return true;
}

bool MoveRecognizer::isEnPassant() {
	// bicie w przelocie: pionek koloru wykonuj¹cego ruch przesuwa siê za pion
	// koloru przeciwnego, który prawdopodobnie w poprzednim ruchu
	// przesun¹³ siê o dwa pola w przód (pionek ten zostaje zabrany)
	
	if (orderedChanges[MOVED_FIGURE_COLOR].size() != 1 ||
		orderedChanges[SECOND_FIGURE_COLOR].size() != 1 ||
		orderedChanges[EMPTY_FIELD].size() != 1)
			return false;
	if (orderedChanges[MOVED_FIGURE_COLOR][0].colorNewState != emptyField ||
		orderedChanges[SECOND_FIGURE_COLOR][0].colorNewState != emptyField || 
		orderedChanges[EMPTY_FIELD][0].colorNewState != movedFigureColor)
			return false;
	if (abs(orderedChanges[MOVED_FIGURE_COLOR][0].coords.x - orderedChanges[SECOND_FIGURE_COLOR][0].coords.x) != 1 ||
		abs(orderedChanges[EMPTY_FIELD][0].coords.y - orderedChanges[SECOND_FIGURE_COLOR][0].coords.y) != 1 ||
		orderedChanges[EMPTY_FIELD][0].coords.x != orderedChanges[SECOND_FIGURE_COLOR][0].coords.x)
			return false;

	moveCoords->push_back(orderedChanges[MOVED_FIGURE_COLOR][0].coords);
	moveCoords->push_back(orderedChanges[EMPTY_FIELD][0].coords);

	printf("MoveRecognizer: rozpoznano bicie w przelocie: %c%d => %c%d\n",
		'A'+(*moveCoords)[0].x, 1+(*moveCoords)[0].y, 'A'+(*moveCoords)[1].x, 1+(*moveCoords)[1].y);
	return true;
}