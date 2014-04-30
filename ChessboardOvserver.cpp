#include "ChessboardObserver.h"

using namespace cv;

extern const char* OUT_WINDOW_NAME = "chessboard";
extern const char* HAND_WND_NAME = "hand detection";
extern const char* TRACKBARS_WND_NAME = "detection config";
extern const char* FIGURES_FINDING_WND_NAME = "figures finding";
extern const char* FIGURES_FINDING_CANNY_WND_NAME = "canny (figures)";

extern int calibrationOn = 1;
extern int drawWindowsLevel = 1;
extern int drawResult = 1;
extern int globalContrast = 10;
extern int autoContrast = 1;


// ostatni ruch - dla rysowania strza�ki
vector<Point2i> moveCoords;
bool moveDetected;



void onDrawWindows(int value, void* data) {
	static int oldValue = drawWindowsLevel;
	// nowa warto�� drawWindowsLevel jest ustawiana na value wcze�niej

	if (value < oldValue) {
		if (value <= 1) {
			destroyWindow(HAND_WND_NAME);
			destroyWindow(FIGURES_FINDING_CANNY_WND_NAME);
		}
		if (value < 1) {
			destroyWindow(FIGURES_FINDING_WND_NAME);
		}
	}

	oldValue = value;
}



ChessboardObserver::ChessboardObserver(VideoCapture cap) {
	this->cap = cap;
	this->figuresFinder.setChessboardDetector(&chessboardDetector);
	
#if SMALL_RESOLUTION
	// rozdzielczo�ci tej kamery: 640x480, 352x288, 320x240, 176x144
	// inne s� zamieniane na najbli�sz� z powy�szych
	this->cap.set(CV_CAP_PROP_FRAME_WIDTH, 352);
	this->cap.set(CV_CAP_PROP_FRAME_HEIGHT, 288);
#endif
}

ChessboardObserver::~ChessboardObserver() {
}

void ChessboardObserver::start() {
	namedWindow(OUT_WINDOW_NAME);
}

DetectionState ChessboardObserver::checkState() {
	DetectionState state = DetectionState::UNCHANGED;

	cap >> frame;

	if (globalContrast != 10)
		frame.convertTo(frame, -1, globalContrast * 0.1, 0);

	if (calibrationOn) {
		moveDetected = false;
		chessboardDetector.detect(&frame);
		figuresFinder.performOnCalibrating(&frame);
	}

	else {
		if (figuresFinder.getIsStarted() == false)
			figuresFinder.start();

		state = figuresFinder.perform(&frame);
		if (state == DetectionState::MOVE) {
			moveDetected = true;
			figuresFinder.getLastMove(moveCoords);
		}
	}

	if (autoContrast && chessboardDetector.chessboardFound() && !figuresFinder.getIsHand()) {
		StateDetector* det = calibrationOn ?
			figuresFinder.getCalibrationStateDetector() :
			figuresFinder.getStateDetector();
		State_t* state = calibrationOn? det->getState() : figuresFinder.getCurrentState();
		estimateContrast(state, det->getLastFrameAsGray());
	}

	drawResults();
	return state;
}

void ChessboardObserver::drawResults() {
	if (drawResult) {
		Mat out;
		frame.copyTo(out);

		// rysowanie strza�ki gdy by� ruch
		if (moveDetected) {
			for (int i=0; i<moveCoords.size(); i++) {
				Point2i from = moveCoords[i];
				Point2i to = moveCoords[++i];	// zawsze para wsp�rz�dnych
				Point p1 = chessboardDetector.getFieldCenter(from.x, from.y);
				Point p2 = chessboardDetector.getFieldCenter(to.x, to.y);
				line(out, p1, p2, Scalar(255,255,128), 10);
			}
		}

		figuresFinder.drawFigures(out);
		chessboardDetector.drawGrid(out);
		chessboardDetector.drawCornersContours(out);

		imshow(OUT_WINDOW_NAME, out);
	}
	else
		imshow(OUT_WINDOW_NAME, frame);
}

void ChessboardObserver::estimateContrast(void* statePtr, Mat& grayImage) {
	// �rednia warto�� kontrastu pustych p�l
	int sumIntens[2] = { 0, 0 };
	int count[2] = { 0, 0 };

	if (statePtr == NULL) {
		puts("state == NULL w ChessboardObserver::estimateContrast()");
		return;
	}
	typedef char state_t[8][8];
	state_t* state = (state_t*) statePtr;

	for (int w=0; w<8; w++) {
		for (int c=0; c<8; c++) {

			if ((*state)[w][c] == 0) {
				int intensity = chessboardDetector.getFieldIntensity(grayImage, c, w);
				if (intensity < 0)
					continue;
				sumIntens[(w+c)&1] += intensity;
				count[(w+c)&1]++;

				continue;
			}
		}
	}

	if (count[0] && count[1]) {
		int avgWhite = sumIntens[1] / count[1];
		int avgBlack = sumIntens[0] / count[0];

		int contrastDelta = 0;
		
		if (avgBlack > 190)
			contrastDelta = - max(1, (avgBlack-190)/5);
		else if (avgWhite < 250)
			contrastDelta = max(1, (255-avgWhite)/30);
		else if (avgBlack < 170)
			contrastDelta = max(1, (190-avgBlack)/30);

		if (contrastDelta) {
			figuresContrast += contrastDelta;
			setTrackbarPos("contrast", TRACKBARS_WND_NAME, figuresContrast);
		
			/*printf("Avg empty intensity: white: %d (count: %d) black: %d (%d); new contrast: %d\n",
				avgWhite, count[1], avgBlack, count[0], figuresContrast);*/
		}
	}
}

bool ChessboardObserver::undoneMove() {
	if (figuresFinder.undoneMove()) {
		puts("Przywrocono poprzedni stan szachownicy.");
		printf("Teraz ruch %s.\n", figuresFinder.getIsWhiteFiguresMove() ? "bialych" : "czarnych");
		moveCoords.clear();
		return true;
	} else {
		puts("Nie udalo sie przywrocic poprzedniego stanu szachownicy.");
		return false;
	}
}
