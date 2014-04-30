#include "FiguresFinder.h"
#include "MoveRecognizer.h"
#include <vector>
#include <cstdio>

using namespace cv;

int minHandArea = 360;


vector<vector<Point> > contours, selectedContours;
vector<Vec4i> hierarchy;
// subtrakcja t³a - dla wykrywania rêki
// MOG2 jest znacznie szybszy od MOG i chyba lepiej dzia³a
Ptr<BackgroundSubtractorMOG2> mog;

vector<MoveRecognizer::ChangeInfo> changes;


FiguresFinder::FiguresFinder(ChessboardDetector* chessboardDetector) {
	isPlayerMove = false;
	isHand = false;
	isStateOK = true;
	isWhiteFiguresMove = true;
	isMoveRecognized = false;
	isStarted = false;
	memset(lastState, 0, sizeof(lastState));
	memset(currentState, 0, sizeof(currentState));
	memset(newState, 0, sizeof(newState));

	calibrationStateDetector = new StateDetector(chessboardDetector, 1, 1);

	if (drawWindowsLevel >= 2) {
		namedWindow(HAND_WND_NAME,1);
		createTrackbar("min hand", HAND_WND_NAME, &minHandArea, 9000);
	}
	
	// drugi parametr: 16 to typowa wartoœæ; musi to byæ potêga
	mog = new BackgroundSubtractorMOG2(100, 16, false);
	
	stateDetector = new StateDetector(chessboardDetector, 7, 4);
	setChessboardDetector(chessboardDetector);
}

FiguresFinder::~FiguresFinder() {
	delete stateDetector;
	delete calibrationStateDetector;
}

void FiguresFinder::performOnCalibrating(Mat* frame) {
	if (calibrationStateDetector->detect(frame))
        memcpy(calibrationState, calibrationStateDetector->getState(), sizeof(calibrationState));
	isHand = false;
	if (stateDetector->isDetecting())
		stateDetector->stop();
}

DetectionState FiguresFinder::perform(Mat* frame) {
	lastFrame = frame;
	bool stateDetected = false;
	isMoveRecognized = false;

	if (chessboardDetector->chessboardFound() == false)
        return IMPOSSIBLE;

	if (isHand != detectHand())
		puts(isHand? "Hand detected" : "Hand disappeared");

	if (isPlayerMove) {
		if (!isHand) {
//			puts("End of move");
			// rozpocznij wykrywanie figur na kilku kolejnych ramkach
			stateDetector->detect(lastFrame);
			puts(" stateDetector starts");
			isPlayerMove = false;
			stateDetected = true;
		}
	}

	else {
		if (isHand) {
//			puts("Begin of move");
			if (stateDetector->isDetecting()) {
				stateDetector->stop();
				// pomijamy nie w pe³ni wykryty stan - byæ mo¿e to b³¹d wykrywania rêki
				puts(" stateDetector stops - because hand");
				stateDetected = true;
			}
			isPlayerMove = true;
		}
		else {
			if (stateDetector->isDetecting()) {
				if (stateDetector->detect(lastFrame)) {
//					puts(" stateDetector ends");

					// jeœli poprzedni stan by³ z³y, nie wykrywaj nowego -
					// - uznaj, ¿e ten ruch by³ popraw¹ pionków

					if (isStateOK == false) {

						if (ChessboardState::compare(stateDetector->getState(), currentState)) {
							isStateOK = true;

							puts("\nPrzywrocono poprzedni stan. Mozna teraz powtorzyc ruch.");
							printf("Teraz ruch %s.\n\n", isWhiteFiguresMove ? "bialych" : "czarnych");
						}
						else {
							printf("\nWciaz nie przywrocono poprzedniego ustawienia figur\npo niepoprawnym ruchu %s.\n",
								isWhiteFiguresMove ? "bialych" : "czarnych");
						}
					}

					else {
						determineNewState();

						if (isStateOK == false) {
							// nie rozponano poprawnego ruchu - nie zapisano nowego stanu
							puts("\nNie rozpoznano poprawnego ruchu - powroc do poprzedniego ustawienia figur.\n");
						}

						else if (isMoveRecognized) {
							isWhiteFiguresMove = !isWhiteFiguresMove;
							printf("\nWykonano ruch. Teraz ruch %s.\n\n", isWhiteFiguresMove ? "bialych" : "czarnych");
						}
					}
					
					stateDetected = true;
				}
			}
			else {
				// przygotuj ramkê w odcieniu szaroœci - potrzebna dla automatycznego ustawiania kontrastu
				if (autoContrast)
					stateDetector->prepareGrayFrame(lastFrame);
			}
		}
	}

	if (isStateOK == false)
        return INCORRECT;
	else if (isMoveRecognized)
        return MOVE;
	else
        return UNCHANGED;
}

bool FiguresFinder::detectHand() {
	Mat fgmask;
	Mat* frame = lastFrame;

	// ostatni parametr: raczej wiêkszy od 0
	mog->operator()(*frame, fgmask, 0.1);

	// transformacja otwarcia: erozja i dylatacja - aby pozbyc sie szumu
	erode(fgmask, fgmask, Mat());
	dilate(fgmask, fgmask, Mat());

	// tylko zewnêtrzne kontury, wszystkie punkty konturu
	findContours(fgmask, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	// wybór konturów które s¹ wystarczaj¹co du¿e - odsiewamy drobne przypadkowe zmiany
	selectedContours.clear();
	for (unsigned int i=0; i<contours.size(); i++) {
		int area = (int) contourArea(contours[i]);
		if (area >= minHandArea)
			selectedContours.push_back(contours[i]);
	}

	isHand = selectedContours.size() > 0;

	if (drawWindowsLevel >= 2) {
		imshow(HAND_WND_NAME, fgmask);
		drawContours(*frame, selectedContours, -1, Scalar(255,0,0), 3);
	}

	return isHand;
}

void FiguresFinder::determineNewState() {
	memcpy(newState, stateDetector->getState(), sizeof(newState));
	if (detectFigureMove()) {
		memcpy(lastState, currentState, sizeof(lastState));
		//memcpy(currentState, newState, sizeof(lastState));
		// nie polegamy œciœle na wykrytym nowym stanie, bo coœ mo¿e byæ b³êdnie wykryte
		// zamiast tego ustawiamy tylko zmiany wykryte przez wykrywanie ruchu
		for (int i=0; i<changes.size(); i++) {
			Point2i& move = changes[i].coords;
			currentState[move.y][move.x] = changes[i].colorNewState;
		}

		statesHistory.push_back(new ChessboardState(lastState));
		if (statesHistory.size() > MOVE_HISTORY_SIZE) {
			delete statesHistory.front();
			statesHistory.pop_front();
		}
	}
}

bool FiguresFinder::detectFigureMove() {
	MoveRecognizer moveRecognizer;
	MoveRecognizer::MoveType result;

	changes.clear();
	
	puts("\nMove detection:");

	for (int y=0; y<8; y++) {
		for (int x=0; x<8; x++) {

			if (newState[y][x] == currentState[y][x])
				continue;

			// je¿eli jest ruch np. bia³ych, to wiadomo, ¿e na ¿adnym polu
			// nie mog³a siê pojawiæ czarna figura
			if (newState[y][x] == (isWhiteFiguresMove? 'b' : 'w')) {
				printf("Niepoprawna zmiana: [%d][%d] (%c%d) %c ==> %c. Odrzucam.\n", y, x, 'A'+x, 1+y, currentState[y][x], newState[y][x]);
				continue;
			}

			printf("Zmiana: [%d][%d] (%c%d) %c ==> %c\n", y, x, 'A'+x, 1+y, currentState[y][x], newState[y][x]);
			MoveRecognizer::ChangeInfo changeInfo = { Point2i(x,y), currentState[y][x], newState[y][x] };
			changes.push_back(changeInfo);
		}
	}

	result = moveRecognizer.recognize(changes, isWhiteFiguresMove, moveCoords);
    if (result == INCORRECT) {
		// niepoprawny stan planszy lub nierozpoznany ruch
		isStateOK = false;
		isMoveRecognized = false;
	}
    else if (result == MoveRecognizer::NO_MOVE) {
		// stan jest zgodny z poprzednim - nie wykryto ruchu
		// zak³adamy ¿e stan jest poprawny - bo poprzedni by³ poprawny
		isStateOK = true;
		isMoveRecognized = false;
	}
	else {
		// rozpoznano wykonany ruch figury
		isStateOK = true;
		isMoveRecognized = true;
	}

	return isMoveRecognized;
}

void FiguresFinder::drawFigures(Mat& image) {
	if (chessboardDetector == NULL || !chessboardDetector->chessboardFound())
		return;

	Scalar white(255,255,255);
	Scalar black(0,0,255);	// na czerwono bo czarnego przy s³abym swietle nie widaæ
	State_t* state;

	if (calibrationOn)
		state = calibrationState;
	else
		state = currentState;
	
	for (int w=0; w<8; w++) {
		for (int c=0; c<8; c++) {

			if (state[w][c] == 0)
				continue;
			
			bool isWhite = ( state[w][c] == 'w' );

			Point fieldCenter = chessboardDetector->getFieldCenter(c, w);
			circle(image, fieldCenter, 12, (isWhite? white : black), -1);
		}
	}
}

void FiguresFinder::getLastMove(vector<Point2i>& moveCoords) {
	moveCoords = this->moveCoords;
}

void FiguresFinder::setChessboardDetector(ChessboardDetector* chessboardDetector) {
	this->chessboardDetector = chessboardDetector;
	this->stateDetector->setChessboardDetector(chessboardDetector);
	this->calibrationStateDetector->setChessboardDetector(chessboardDetector);
}

void FiguresFinder::start() {	
	for (int w=0; w<8; w++) {
		currentState[0][w] = 'w';
		currentState[1][w] = 'w';
		currentState[6][w] = 'b';
		currentState[7][w] = 'b';
	}
	
	isStarted = true;

	puts("\nStart gry. Teraz ruch bialych.\n");
}

bool FiguresFinder::undoneMove() {
	if (statesHistory.size() == 0)
		return false;

	memcpy(currentState, statesHistory.back()->getState(), sizeof(currentState));
	statesHistory.pop_back();
	if (statesHistory.size() > 0)
		memcpy(lastState, statesHistory.back()->getState(), sizeof(lastState));
	else
		memset(lastState, 0, sizeof(lastState));

	isWhiteFiguresMove = !isWhiteFiguresMove;
	return true;
}
