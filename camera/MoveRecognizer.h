#ifndef MOVE_RECOGNIZER_H
#define MOVE_RECOGNIZER_H

#include "ChessboardDetection.h"



/**
 * Lekka klasa której zadaniem jest rozpoznanie jaki ruch zosta³ wykonany (o ile
 * zosta³ wykonany) na podstawie przekazanych zmian na szachownicy.
 */
class MoveRecognizer
{
public:

	/**
	 * Typ wykonanego ruchu (lub b³¹d, jeœli zmiany na szachownicy nie wskazuj¹
	 * na poprawny ruch. */
	enum MoveType {
		INCORRECT,		/// niepoprawny lub nierozpoznany ruch lub braku ruchu
		MOVE,			/// przesuniêcie figury na wolne pole
		CAPTURING,		/// bicie
		CASTLING,		/// roszada
		EN_PASSANT,		/// bicie w przelocie
		NO_MOVE			/// nie wykonano ¿adnego ruchu (stan siê nie zmieni³)
	};
	
	/**
	 * Struktura przechowuj¹ca informacje o zmianie stanu pola szachownicy. */
	struct ChangeInfo {
		cv::Point2i coords;		/// wspó³rzêdne w uk³adzie szachownicy <0;7>
		char colorLastState;	/// kolor figury w starym stanie (w/b/0)
		char colorNewState;		/// wykryty kolor figury w nowym satnie (w/b/0)
	};
	

	/**
	 * Sprawdza jaki PRAWDOPODOBNIE zosta³ wykonany ruch.
	 * Na tym etapie nie jest mo¿liwe sprawdzenie czy to rzeczywiœcie ten ruch,
	 * tym zajmuje sie modu³ logiczny. Tutaj sprawdza, czy liczba i kolory
	 * pionków odpowiadaj¹ konkretnemu ruchowi.
	 * Do wektora moveCoords zostan¹ wpisane zmiany zgodnie z opisem
	 * metody getMoveCoords() w klasie FiguresFinder. */
	MoveType recognize(std::vector<ChangeInfo>& changes, bool whiteFiguresMove, std::vector<cv::Point2i>& moveCoords);

private:
	// zmiany kolejno dla: koloru wykonuj¹cego ruch, drugiego koloru, pustego pola
	std::vector<ChangeInfo> orderedChanges[3];
	char movedFigureColor, secondFigureColor, emptyField;
	std::vector<cv::Point2i>* moveCoords;

	/** Sprawdzenie czy ruch jest przesuniêciem figury na wolne pole. */
	bool isMove();
	/** Sprawdzenie czy ruch jest biciem (ale nie w przelocie). */
	bool isCapturing();
	/** Sprawdzenie czy ruch jest roszad¹. */
	bool isCastling();
	/** Sprawdzenie czy ruch jest biciem w przelocie. */
	bool isEnPassant();

	enum FigureColors {
		MOVED_FIGURE_COLOR = 0,
		SECOND_FIGURE_COLOR = 1,
		EMPTY_FIELD = 2
	};

};


#endif // MOVE_RECOGNIZER_H