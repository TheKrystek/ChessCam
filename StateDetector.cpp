#include "StateDetector.h"
#include <list>

using namespace cv;
using std::list;


extern int figuresContrast = 10;
extern int whiteFigureThreshold = 90;

int edgeLineSize = 11;
int drawEdgeLines = 0;

list<StateDetector*> detectorsContainer;


void onContrastChange(int value, void* object) {
	list<StateDetector*>::iterator iter = detectorsContainer.begin();
	for ( ; iter != detectorsContainer.end(); iter++)
		((StateDetector*)*iter)->setContrast(value);
}

void onWhiteThreshChange(int value, void* object) {
	list<StateDetector*>::iterator iter = detectorsContainer.begin();
	for ( ; iter != detectorsContainer.end(); iter++)
		((StateDetector*)*iter)->setWhiteFigureThreshold(value);
}



StateDetector::StateDetector(ChessboardDetector* chessboardDetector, int frames, int threshold) {
	this->frames = frames;
	this->threshold = threshold;
	this->chessboardDetector = chessboardDetector;
	contrast = figuresContrast;
	whiteFigureThreshold = ::whiteFigureThreshold;
	detecting = false;
	memset(state, 0, sizeof(state));
	
	if (drawWindowsLevel >= 1)
		namedWindow(FIGURES_FINDING_WND_NAME);
	if (drawWindowsLevel >= 2)
		namedWindow(FIGURES_FINDING_CANNY_WND_NAME);

	detectorsContainer.push_back(this);
}

StateDetector::~StateDetector() {
	detectorsContainer.remove(this);
}

bool StateDetector::detect(Mat* frame) {
	if (detecting == false) {
		// start wykrywania
		currentFrameNum = 0;
		memset(state, 0, sizeof(state));
		memset(counter, 0, sizeof(counter));
		detecting = true;
	}

	currentFrame = frame;
	findFigures(*currentFrame, gray, counter);
	currentFrameNum++;

	if (currentFrameNum >= frames) {
		// zakoñczono wykrywanie
		stop();
		return true;
	}

	return false;
}

void StateDetector::stop() {
	int threshold;

	// tablica state jest wyzerowana wczeœniej

	if (currentFrameNum >= frames)
		threshold = this->threshold;
	else {
		threshold = (int) (currentFrameNum / frames) * this->threshold;
		if (threshold < 1)
			threshold = 1;
	}

	for (int w=0; w<8; w++) {
		for (int c=0; c<8; c++) {
			if (counter[w][c] >= threshold)
				state[w][c] = 1;
		}
	}

	determineColors();

	detecting = false;
	currentFrameNum = 0;
}

void StateDetector::findFigures(Mat& frame, Mat& gray, char counter[8][8]) {
	Mat afterContrast, canny, contoursImg;
	vector< vector<Point> > contours, figuresContours;
	vector<Vec4i> hierarchy;
	char currentState[8][8] = { 0 };

	vector<Point> positions;
	vector<Rect> figuresBoxes;

	const int CANNY_MIN = 50;
	const int CANNY_MAX = 3 * CANNY_MIN;

	if (chessboardDetector == NULL)
		return;

	prepareGrayFrame(&frame, &gray);

	GaussianBlur(gray, gray, Size(5,5), 0, 0);

	Canny(gray, canny, CANNY_MIN, CANNY_MAX);
	// rysujemy czarn¹ siatkê aby przykryæ krawêdzie pól
	Size fieldSize = chessboardDetector->getFieldSize();
	int edgeLineSize = fieldSize.width / 5;
	if (edgeLineSize > 0)
		chessboardDetector->drawGrid(canny, edgeLineSize, Scalar(0,0,0));

	canny.copyTo(contoursImg);
	findContours(contoursImg, contours, hierarchy, RETR_LIST, CHAIN_APPROX_SIMPLE);

	// wybierz figury

	figuresContours.clear();
	int minArea = 50;	// 100
	int maxArea = (fieldSize.width-edgeLineSize)*(fieldSize.width-edgeLineSize);	// 1000
	vector<Point> approx;

	for (int i=0; i<contours.size(); i++) {
		Rect boundRect = boundingRect(contours[i]);
		int area = boundRect.area();
				
		if (area > maxArea || area < minArea)
			continue;
				
		Point pos = Point(boundRect.x+boundRect.width/2, boundRect.y+boundRect.height/2);

		Point figurePos;
		if (chessboardDetector->worldToChessboard(pos, figurePos) == false)
			continue;

		if (currentState[figurePos.y][figurePos.x])
			// pomiñ pole na którym ju¿ wykryto figurê
			continue;

		currentState[figurePos.y][figurePos.x] = 1;
		++counter[figurePos.y][figurePos.x];

		// dla rysowania
		positions.push_back(pos);
		figuresBoxes.push_back(boundRect);
	}

	if (drawWindowsLevel >= 1) {
		Mat result;
		cvtColor(gray, result, COLOR_GRAY2BGR);

		for (int b=0; b<figuresBoxes.size(); b++)
			rectangle(result, figuresBoxes[b].tl(), figuresBoxes[b].br(), Scalar(0,0,255));

		for (int i=0; i<positions.size(); i++)
			rectangle(result, Point(positions[i].x-1,positions[i].y-1), Point(positions[i].x+1,positions[i].y+1), Scalar(0,255,0),2);
	
		if (drawEdgeLines && edgeLineSize > 0)
			chessboardDetector->drawGrid(result, edgeLineSize, Scalar(0,0,255));

		imshow(FIGURES_FINDING_WND_NAME, result);
	}
	if (drawWindowsLevel >= 2)
		imshow(FIGURES_FINDING_CANNY_WND_NAME, canny);
}

void StateDetector::findFigures(Mat& frame) {
	Mat gray;
	char counter[8][8];
	findFigures(frame, gray, counter);
}

void StateDetector::determineColors() {
	for (int w=0; w<8; w++) {
		for (int c=0; c<8; c++) {

			if (state[w][c] == 0)
				continue;

			int intensity = chessboardDetector->getFieldIntensity(gray, c, w);
			if (intensity < 0) {
				state[w][c] = 0;
				continue;
			}

			state[w][c] = ( intensity < whiteFigureThreshold ? 'b' : 'w' );
		}
	}
}

void StateDetector::setChessboardDetector(ChessboardDetector* chessboardDetector) {
	this->chessboardDetector = chessboardDetector;
}

void StateDetector::prepareGrayFrame(Mat* frame, Mat* grayFrame) {
	Mat afterContrast, *tmpFrame;
	if (contrast != 10) {
		frame->convertTo(afterContrast, -1, contrast * 0.1, 0);
		tmpFrame = &afterContrast;
	}
	else
		tmpFrame = frame;

	if (grayFrame)
		cvtColor(*tmpFrame, *grayFrame, COLOR_BGR2GRAY);
	else
		cvtColor(*tmpFrame, gray, COLOR_BGR2GRAY);
}