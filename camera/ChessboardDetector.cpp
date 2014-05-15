#include "ChessboardDetector.h"
#include <cstdio>
using namespace cv;

//typedef Scalar_<int> Scalar3i;


static vector<vector<Point> > contours;


// parametry barwy HSV dla wykrywania markerów po³o¿onych na rogach szachownicy
// kolor czerwony
//Scalar3i markerLower(0, 180, 192);
//Scalar3i markerUpper(51, 255, 255);
// kolor ¿ó³ty
//Scalar3i markerLower(0, 175, 95);
//Scalar3i markerUpper(55, 255, 255);
// zielony
//Scalar3i markerLower(40, 65, 15);
//Scalar3i markerUpper(95, 255, 255);
// niebieski
//Scalar3i markerLower(55, 100, 5);
//Scalar3i markerUpper(145, 255, 255);

// niebieskie drukowane na szachownicy czerwono-bia³ej
// du¿y przedzial - dla ró¿nych warunków oœwietlenia
extern Scalar3i markerLower(70, 60, 0);
extern Scalar3i markerUpper(145, 255, 200);


ChessboardDetector::ChessboardDetector() {
	corners.resize(4);
	cornersFound = false;
	avgFieldSize.width = 0;
	avgFieldSize.height = 0;
}

ChessboardDetector::~ChessboardDetector() {
}


bool ChessboardDetector::detect(Mat* frame) {
	lastFrame = frame;

	findCorners(CORNERS_DETECTION_CORNER);
	if (cornersFound) {
		determineLines();
		calculateAvgFieldSize();
	}

	return cornersFound;
}

// Pomocnicza funkcja szukaj¹ca podobszaru szachownicy w którym znajduje siê punkt
// poprzez przeszukiwanie binarne. Argument lines to verticalLines lub horizontalLines
// a area sk³ada siê z 4 punktów (polygon, niekoniecznie prostok¹t).
int findPointInArea(Point2f point, vector<Point2f>& area, Point2f lines[2][7]) {
	if (pointPolygonTest(area, point, false) > 0) {
		area[1] = lines[0][1];
		area[2] = lines[1][1];

		if (pointPolygonTest(area, point, false) > 0) {
			area[1] = lines[0][0];
			area[2] = lines[1][0];
			return (pointPolygonTest(area, point, false) > 0) ? 0 : 1;
		}
		else {
			area[0] = lines[0][2];
			area[3] = lines[1][2];
			return (pointPolygonTest(area, point, false) > 0) ? 2 : 3;
		}
	}

	else {
		area[0] = lines[0][5];
		area[3] = lines[1][5];

		if (pointPolygonTest(area, point, false) > 0) {
			area[0] = lines[0][4];
			area[3] = lines[1][4];
			return (pointPolygonTest(area, point, false) > 0) ? 4 : 5;
		}
		else {
			area[1] = lines[0][6];
			area[2] = lines[1][6];
			return (pointPolygonTest(area, point, false) > 0) ? 6 : 7;
		}
	}
}

bool ChessboardDetector::worldToLocal(Point2f pointOnImage, Point2i& location) {
	// Jeœli punkt le¿y poza szachownic¹, zwróæ false.
	// pointPolygonTest zwraca 0 jeœli punkt le¿y na linii, wartoœæ dodatni¹
	// jeœli w obszarze lub wartoœæ ujemn¹ jeœli poza obszarem.
	if (pointPolygonTest(corners, pointOnImage, false) < 0)
		return false;

	/* sposób 1: sprawdzanie czy punkt le¿y w obszarze pól */
	
	vector<Point2f> area(4);
	area[0] = corners[LeftTop];
	area[1] = verticalLines[0][3];
	area[2] = verticalLines[1][3];
	area[3] = corners[LeftBottom];

	location.x = findPointInArea(pointOnImage, area, verticalLines);
	
	area[0] = corners[LeftTop];
	area[1] = horizontalLines[0][3];
	area[2] = horizontalLines[1][3];
	area[3] = corners[RightTop];
	
	location.y = findPointInArea(pointOnImage, area, horizontalLines);

	return true;
}

// Finds the intersection of two lines, or returns false.
// The lines are defined by (o1, p1) and (o2, p2).
bool intersection(Point2f o1, Point2f p1, Point2f o2, Point2f p2, Point2f &r) {
	Point2f x = o2 - o1;
	Point2f d1 = p1 - o1;
	Point2f d2 = p2 - o2;

	float cross = d1.x*d2.y - d1.y*d2.x;
	if (abs(cross) < /*EPS*/1e-8)
		return false;

	double t1 = (x.x * d2.y - x.y * d2.x)/cross;
	r = o1 + d1 * t1;
	return true;
}

vector<Point2f> ChessboardDetector::getFieldRect(int x, int y) {
	vector<Point2f> points(4);
	// punkty skrajne linii: lewa, prawa, góra, dó³
	Point2f lines[4][2];

	Point2i local = chessboardToLocal(Point2i(x,y));
	x = local.x;
	y = local.y;

	if (x == 0) {
		lines[0][0] = corners[LeftTop];
		lines[0][1] = corners[LeftBottom];
	}
	else {
		lines[0][0] = verticalLines[0][x-1];
		lines[0][1] = verticalLines[1][x-1];
	}
	
	if (x == 7) {
		lines[1][0] = corners[RightTop];
		lines[1][1] = corners[RightBottom];
	}
	else {
		lines[1][0] = verticalLines[0][x];
		lines[1][1] = verticalLines[1][x];
	}
	
	if (y == 0) {
		lines[2][0] = corners[LeftTop];
		lines[2][1] = corners[RightTop];
	}
	else {
		lines[2][0] = horizontalLines[0][y-1];
		lines[2][1] = horizontalLines[1][y-1];
	}
	
	if (y == 7) {
		lines[3][0] = corners[LeftBottom];
		lines[3][1] = corners[RightBottom];
	}
	else {
		lines[3][0] = horizontalLines[0][y];
		lines[3][1] = horizontalLines[1][y];
	}

	intersection(lines[0][0], lines[0][1], lines[2][0], lines[2][1], points[LeftTop]);
	intersection(lines[1][0], lines[1][1], lines[2][0], lines[2][1], points[RightTop]);
	intersection(lines[1][0], lines[1][1], lines[3][0], lines[3][1], points[RightBottom]);
	intersection(lines[0][0], lines[0][1], lines[3][0], lines[3][1], points[LeftBottom]);
	return points;
}

Rect ChessboardDetector::getFieldInnerRect(int x, int y, int shift) {
	Rect rect;
	vector<Point2f> fieldRect = getFieldRect(x,y);
	rect.x = max(fieldRect[LeftTop].x, fieldRect[LeftBottom].x) + shift;
	rect.y = max(fieldRect[LeftTop].y, fieldRect[RightTop].y) + shift;
	rect.width = min(fieldRect[RightTop].x, fieldRect[RightBottom].x) - rect.x - shift;
	rect.height = min(fieldRect[LeftBottom].y, fieldRect[RightBottom].y) - rect.y - shift;
	return rect;
}

Point ChessboardDetector::getFieldCenter(int x, int y) {
	Point center;
	vector<Point2f> rect = getFieldRect(x, y);
	center.x = (rect[0].x + rect[1].x + rect[2].x + rect[3].x) / 4;
	center.y = (rect[0].y + rect[1].y + rect[2].y + rect[3].y) / 4;
	return center;
}

bool ChessboardDetector::worldToChessboard(Point2f pointOnImage, Point2i& location) {
	Point2i cell;
	if (worldToLocal(pointOnImage, cell) == false)
		return false;
	location = localToChessboard(cell);
	return true;
}

Point2i ChessboardDetector::localToChessboard(Point2i cell) {
	return Point2i(7 - cell.y, 7 - cell.x);
}

Point2i ChessboardDetector::chessboardToLocal(Point2i field) {
	return Point2i(7 - field.y, 7 - field.x);
}

void putChars(Mat& image, Scalar color, Point from, Point to, char first, char last, Point shift) {
	int sign = (last < first) ? -1 : 1;
	Point pt = from;
	const int N = (last - first) * sign + 1;
	int shiftX = (to.x - pt.x) / N;
	int shiftY = (to.y - pt.y) / N;

	pt.x += shift.x + shiftX/2;
	pt.y += shift.y + shiftY/2;

	char ch = first - sign;
	do {
		ch += sign;
		putText(image, string(1, ch), pt, 2, 0.8, color);
		pt.x += shiftX;
		pt.y += shiftY;
	} while (ch != last);
}

void ChessboardDetector::drawGrid(Mat& image, bool printCoords) {
	// osobna implementacja, bo tu malowane s¹ markery i boki o innej szerokoœci
	// ni¿ linie wewnêtrzne

	if (cornersFound == false)
		return;
	
	Scalar color(0,255,0);
	
	// poziome i pionowe linie wewnêtrzne (rozdzielaj¹ pola)
	for (int i=0; i<7; i++) {
		line(image, horizontalLines[0][i], horizontalLines[1][i], color, 1);
		line(image, verticalLines[0][i], verticalLines[1][i], color, 1);
	}

	// rogi (po³o¿enie markerów) oraz boki szachownicy
	for (int i=0; i<4; i++) {
		circle(image, corners[i], 5, color, -1);
		line(image, corners[i], corners[(i+1)%4], color, 2);
	}

	// koordynaty pól
	if (printCoords) {
		Point from, to;
		Size fieldSize = getFieldSize();

		from = Point(corners[LeftTop].x, corners[LeftTop].y);
		to = Point(corners[RightTop].x, corners[RightTop].y);
		putChars(image, color, from, to, '8', '1', Point(0, -10));
		
		from = Point(corners[LeftTop].x, corners[LeftTop].y);
		to = Point(corners[LeftBottom].x, corners[LeftBottom].y);
		putChars(image, color, from, to, 'H', 'A', Point(-25, fieldSize.height/4));
	}
}

void ChessboardDetector::drawGrid(Mat& image, int lineSize, Scalar color) {
	if (cornersFound == false)
		return;
	
	// poziome i pionowe linie wewnêtrzne (rozdzielaj¹ pola)
	for (int i=0; i<7; i++) {
		line(image, horizontalLines[0][i], horizontalLines[1][i], color, lineSize);
		line(image, verticalLines[0][i], verticalLines[1][i], color, lineSize);
	}

	// boki szachownicy - tutaj ta sama szerokoœæ linii co linie wewnêtrzne
	for (int i=0; i<4; i++) {
		line(image, corners[i], corners[(i+1)%4], color, lineSize);
	}
}


void ChessboardDetector::findCorners(int method) {
	// Znajdowanie kszta³tów o znanym kolorze (markery kolorowe na rogach planszy)
	// Na podstawie: http://suksant.com/2013/04/26/fast-colour-based-object-tracking-with-opencv/
	Mat hsvImg, thresh, contoursImg;
	vector<Vec4i> hierarchy;

	GaussianBlur(*lastFrame, hsvImg, Size(5,5), 0);
	cvtColor(hsvImg, hsvImg, COLOR_BGR2HSV);

	// zakresy HSV wykrywanych kolorów; inRange zwróci maskê
    cv::inRange(hsvImg, cv::Scalar(markerLower), Scalar(markerUpper), thresh);
   // hsvImg.copyTo(thresh);
	// dilate pozwala pozbyæ siê drobnych kruszynek
	int dilatationSize = 5;

	Mat element = getStructuringElement(MORPH_RECT, Size(dilatationSize, dilatationSize));
	erode(thresh, thresh, element);
	dilate(thresh, thresh, element);

	thresh.copyTo(contoursImg);
	
	contours.clear();
	findContours(contoursImg, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	if (method == CORNERS_DETECTION_CENTER)
		findCornersByCenters(contours);
	else if (method == CORNERS_DETECTION_CORNER)
		findCornersByCorners(contours);
	else
		return;

	/*if (drawWindowsLevel >= 2)
		imshow(MARKERS_WND_NAME, thresh);*/
}

// po³o¿enie rogu szachownicy: œrodek markera
bool ChessboardDetector::findCornersByCenters(vector<vector<Point> >& contours) {
	// podzia³ konturów na 4 grupy, wskazuj¹ce na rogi w których umieszczono markery
	vector<vector<Point> > groups(4);
	const int HALF_WIDTH = lastFrame->cols / 2;
	const int HALF_HEIGHT = lastFrame->rows / 2;

	for (int i=0; i<contours.size(); i++) {
		Moments moment = moments(contours[i]);
		Point pos(moment.m10 / moment.m00, moment.m01 / moment.m00);

		if (pos.x < HALF_WIDTH) {
			if (pos.y < HALF_HEIGHT)
				groups[LeftTop].push_back(pos);
			else
				groups[LeftBottom].push_back(pos);
		}

		else {
			if (pos.y < HALF_HEIGHT)
				groups[RightTop].push_back(pos);
			else
				groups[RightBottom].push_back(pos);
		}
	}

	cornersFound = true;
	for (int g=0; g<4; g++) {
		if (groups[g].size() == 0) {
			cornersFound = false;
			return false;
		}
	}

	// jeœli rogi nie zosta³y znalezione, stare wartoœci nie zostan¹ nadpisane
	// z testów wynika ¿e znalezionych konturów w grupie jest zwykle 1 lub kilka
	for (int g=0; g<4; g++) {
		Point avg;
		for (int p=0; p<groups[g].size(); p++) {
			avg.x += groups[g][p].x;
			avg.y += groups[g][p].y;
		}
		corners[g] = Point(avg.x / groups[g].size(), avg.y / groups[g].size());
	}

	return true;
}

// po³o¿enie rogu planszy: wewnêrzny róg markera
bool ChessboardDetector::findCornersByCorners(vector<vector<Point> >& contours) {
	// podzia³ konturów na 4 grupy, wskazuj¹ce na rogi w których umieszczono markery
	vector<Point> groups(4);
	const int HALF_WIDTH = lastFrame->cols / 2;
	const int HALF_HEIGHT = lastFrame->rows / 2;

	for (int i=0; i<contours.size(); i++) {
		Rect box = boundingRect(contours[i]);

		if (box.x+box.width < HALF_WIDTH) {

			if (box.y+box.height < HALF_HEIGHT) {
				Point point = box.br();
				if (groups[LeftTop].x == 0)
					groups[LeftTop] = point;
				else {
					if (point.x > groups[LeftTop].x)
						groups[LeftTop].x = point.x;
					if (point.y > groups[LeftTop].y)
						groups[LeftTop].y = point.y;
				}
			}

			else {
				Point point = Point(box.x+box.width, box.y);
				if (groups[LeftBottom].x == 0)
					groups[LeftBottom] = point;
				else {
					if (point.x > groups[LeftBottom].x)
						groups[LeftBottom].x = point.x;
					if (point.y < groups[LeftBottom].y)
						groups[LeftBottom].y = point.y;
				}
			}
		}

		else {
			if (box.y+box.height < HALF_HEIGHT) {
				Point point = Point(box.x, box.y+box.height);
				if (groups[RightTop].x == 0)
					groups[RightTop] = point;
				else {
					if (point.x < groups[RightTop].x)
						groups[RightTop].x = point.x;
					if (point.y > groups[RightTop].y)
						groups[RightTop].y = point.y;
				}
			}

			else {
				Point point = Point(box.x, box.y);
				if (groups[RightBottom].x == 0)
					groups[RightBottom] = point;
				else {
					if (point.x < groups[RightBottom].x)
						groups[RightBottom].x = point.x;
					if (point.y < groups[RightBottom].y)
						groups[RightBottom].y = point.y;
				}
			}
		}
	}

	cornersFound = true;
	for (int g=0; g<4; g++) {
		if (groups[g].x == 0) {
			cornersFound = false;
			return false;
		}
	}

	for (int g=0; g<4; g++)
		corners[g] = groups[g];

	return true;
}

// pomocnicza procedurka - wyznacza punkty skrajne wewnêtrznych linii na jednej krawêdzi
void _determineLines(Point2f start, Point2f end, Point2f linePoints[]) {
	float dx, dy, x, y;

	x = start.x;
	y = start.y;
	dx = (end.x - x) / 8.f;
	dy = (end.y - y) / 8.f;
	for (int i=1; i<=7; i++) {
		linePoints[i-1].x = x + i*dx;
		linePoints[i-1].y = y + i*dy;
	}
}

void ChessboardDetector::determineLines() {
	// zak³adamy ¿e rogi zosta³y poprawnie wyznaczone (cornersFound == true sprawdzone wczeœniej)

	// górna krawêdŸ
	_determineLines(corners[LeftTop], corners[RightTop], verticalLines[0]);

	// dolna krawêdŸ
	_determineLines(corners[LeftBottom], corners[RightBottom], verticalLines[1]);
	
	// lewa krawêdŸ
	_determineLines(corners[LeftTop], corners[LeftBottom], horizontalLines[0]);

	// dolna krawêdŸ
	_determineLines(corners[RightTop], corners[RightBottom], horizontalLines[1]);
}

void ChessboardDetector::calculateAvgFieldSize() {
	avgFieldSize.width =
		(corners[RightTop].x - corners[LeftTop].x +
		corners[RightBottom].x + corners[LeftBottom].x) / 16;
	avgFieldSize.height =
		(corners[LeftBottom].y - corners[LeftTop].y +
		corners[RightBottom].y + corners[RightTop].y) / 16;
}

void ChessboardDetector::drawCornersContours(Mat& img) {
	if (cornersFound == false)
		return;
	drawContours(img, contours, -1, Scalar(255,0,0), 2);
}

int ChessboardDetector::getFieldIntensity(Mat& grayImage, int x, int y) {
	Mat image_roi;
#if HUGE_CHESSBOARD
	const int SHIFT = getFieldSize().width / 4;
#else
	const int SHIFT = getFieldSize().width / 8;
#endif
	try {
		Rect roi = getFieldInnerRect(x, y, SHIFT);
		image_roi = grayImage(roi);
	}
	catch (...) {
        puts("StateDetector::determineColors() - problem with roi");
		return -1;
	}

	return (int) mean(image_roi).val[0];
}
