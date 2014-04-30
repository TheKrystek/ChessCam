#ifndef CHESSBOARD_DETECTOR_H
#define CHESSBOARD_DETECTOR_H

#include "ChessboardDetection.h"
#include <vector>

/// sposoby detekcji rogów
#define CORNERS_DETECTION_CENTER	0	/// œrodek wykrytego markera
#define CORNERS_DETECTION_CORNER	1	/// wewnêtrzny róg wykrytego markera


/**
 * Klasa odpowiedzialna za rozpoznawanie po³o¿enia szachownicy.
 * Pozwala te¿ wyznaczaæ pole szachownicy na którym le¿y znaleziona figura.
 */
class ChessboardDetector
{
public:
	ChessboardDetector();
	~ChessboardDetector();

	/** Podejmij próbê znalezienia szachownicy na obrazie wejœciowym.
	 * Jesli rogi szachownicy nie zostan¹ znalezione, poprzednie znalezione
	 * po³o¿enie rogów nie zostanie nadpisane. */
	bool detect(cv::Mat* frame);

	bool chessboardFound() const						{ return cornersFound; }

	enum CornerType {
		LeftTop, LeftBottom, RightBottom, RightTop
	};

	/**
	 * Pobierz po³o¿enie wykrytych rogów szachownicy (od lewego górnego
	 * poprzez kolejne zgodnie z ruchem wskazówek zegara - patrz: CornerType).
	 * Jeœli ostatnia próba znalezienia szachownicy na obrazie nie powiod³a siê,
	 * zwrócone po³o¿enia wierzcho³kiów bêd¹ nieaktualne. */
	std::vector<cv::Point2f> getCorners() {
		return std::vector<cv::Point2f>(corners);
	}

	/**
	 * Zamieñ wspó³rzêdne obrazu na wspó³rzêdne planszy (z zakresu 0-7).
	 * Zwraca false jeœli punkt znajduje siê poza wykryt¹ szachownic¹. */
	bool worldToChessboard(cv::Point2f pointOnImage, cv::Point2i& fieldLocation);

	/**
	 * Zamiana wspó³rzêdnych uk³adu obraz na lokalny uk³ad szachownicy (uk³ad 
	 * rysowanej siatki). Zwraca false jeœli punkt jest poza szachownic¹. */
	bool worldToLocal(cv::Point2f pointOnImage, cv::Point2i& cell);

	/**
	 * Zamiana wspó³rzêdnych pola szachownicy na komorkê lokalnego uk³adu. */
	cv::Point2i chessboardToLocal(cv::Point2i field);

	/**
	 * Zamiana punktu lokalnego uk³adu (siatki) na komórkê szachownicy. */
	cv::Point2i localToChessboard(cv::Point2i cell);

	/**
	 * Zwraca rogi pola szachownicy o podanych wspó³rzêdnych.
	 * Wspó³rzêdne x,y indeksowane od 0 (np. pole C4 to x=2, y=3). */
	std::vector<cv::Point2f> getFieldRect(int x, int y);

	/**
	 * Zwraca przybli¿ony prostok¹t wnêtrza pola.
	 * Parametr shift okreœla przesuniêcie w stronê œrodka pola. */
	cv::Rect getFieldInnerRect(int x, int y, int shift = 0);

	/**
	 * Zwraca punkt œrodkowy pola (w uk³adzie przechwyconego obrazu). */
	cv::Point getFieldCenter(int x, int y);

	/**
	 * Zwraca œrednie wymiary pola. */
	cv::Size getFieldSize()								{ return avgFieldSize; }

	/**
	 * Zwraca uœrednion¹ wartoœæ natê¿enia barwy na polu.
	 * Zwraca wartoœæ ujemn¹ w razie problemu z wyliczeniem wartoœci. */
	int getFieldIntensity(cv::Mat& grayImage, int x, int y);

	/** Jeœli szachownica nie zosta³a znaleziona, nic nie zostanie narysowane.*/
	void drawGrid(cv::Mat& image, bool printCoords = true);
	void drawGrid(cv::Mat& image, int lineSize, cv::Scalar color);

	void drawCornersContours(cv::Mat& img) ;
	
protected:
	/**
	 * Po³o¿enie kolejno lewego górnego, prawego górnego, lewego dolnego
	 * i prawego dolnego rogu szachownicy.
	 * W³aœciwie jest to po³o¿enie znalezionych znaczników. */
	std::vector<cv::Point2f> corners;

	bool cornersFound;
	cv::Size avgFieldSize;

	/** Punkty koñcowe odcinków rysowanych wewnêtrznych linii szachownicy.
	 * Pierwszy wymiar: krawêdŸ (góra/dó³ lub lewa/prawa), drugi: punkt. */
	cv::Point2f horizontalLines[2][7];
	cv::Point2f verticalLines[2][7];

	cv::Mat* lastFrame;


	void findCorners(int method);
	void determineLines();
	void calculateAvgFieldSize();
	
	bool findCornersByCenters(std::vector<std::vector<cv::Point> >& contours);
	bool findCornersByCorners(std::vector<std::vector<cv::Point> >& contours);
};

#endif // CHESSBOARD_DETECTOR_H