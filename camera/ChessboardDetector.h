#ifndef CHESSBOARD_DETECTOR_H
#define CHESSBOARD_DETECTOR_H

#include "ChessboardDetection.h"
#include <vector>

/// sposoby detekcji rog�w
#define CORNERS_DETECTION_CENTER	0	/// �rodek wykrytego markera
#define CORNERS_DETECTION_CORNER	1	/// wewn�trzny r�g wykrytego markera


/**
 * Klasa odpowiedzialna za rozpoznawanie po�o�enia szachownicy.
 * Pozwala te� wyznacza� pole szachownicy na kt�rym le�y znaleziona figura.
 */
class ChessboardDetector
{
public:
	ChessboardDetector();
	~ChessboardDetector();

	/** Podejmij pr�b� znalezienia szachownicy na obrazie wej�ciowym.
	 * Jesli rogi szachownicy nie zostan� znalezione, poprzednie znalezione
	 * po�o�enie rog�w nie zostanie nadpisane. */
	bool detect(cv::Mat* frame);

	bool chessboardFound() const						{ return cornersFound; }

	enum CornerType {
		LeftTop, LeftBottom, RightBottom, RightTop
	};

	/**
	 * Pobierz po�o�enie wykrytych rog�w szachownicy (od lewego g�rnego
	 * poprzez kolejne zgodnie z ruchem wskaz�wek zegara - patrz: CornerType).
	 * Je�li ostatnia pr�ba znalezienia szachownicy na obrazie nie powiod�a si�,
	 * zwr�cone po�o�enia wierzcho�ki�w b�d� nieaktualne. */
	std::vector<cv::Point2f> getCorners() {
		return std::vector<cv::Point2f>(corners);
	}

	/**
	 * Zamie� wsp�rz�dne obrazu na wsp�rz�dne planszy (z zakresu 0-7).
	 * Zwraca false je�li punkt znajduje si� poza wykryt� szachownic�. */
	bool worldToChessboard(cv::Point2f pointOnImage, cv::Point2i& fieldLocation);

	/**
	 * Zamiana wsp�rz�dnych uk�adu obraz na lokalny uk�ad szachownicy (uk�ad 
	 * rysowanej siatki). Zwraca false je�li punkt jest poza szachownic�. */
	bool worldToLocal(cv::Point2f pointOnImage, cv::Point2i& cell);

	/**
	 * Zamiana wsp�rz�dnych pola szachownicy na komork� lokalnego uk�adu. */
	cv::Point2i chessboardToLocal(cv::Point2i field);

	/**
	 * Zamiana punktu lokalnego uk�adu (siatki) na kom�rk� szachownicy. */
	cv::Point2i localToChessboard(cv::Point2i cell);

	/**
	 * Zwraca rogi pola szachownicy o podanych wsp�rz�dnych.
	 * Wsp�rz�dne x,y indeksowane od 0 (np. pole C4 to x=2, y=3). */
	std::vector<cv::Point2f> getFieldRect(int x, int y);

	/**
	 * Zwraca przybli�ony prostok�t wn�trza pola.
	 * Parametr shift okre�la przesuni�cie w stron� �rodka pola. */
	cv::Rect getFieldInnerRect(int x, int y, int shift = 0);

	/**
	 * Zwraca punkt �rodkowy pola (w uk�adzie przechwyconego obrazu). */
	cv::Point getFieldCenter(int x, int y);

	/**
	 * Zwraca �rednie wymiary pola. */
	cv::Size getFieldSize()								{ return avgFieldSize; }

	/**
	 * Zwraca u�rednion� warto�� nat�enia barwy na polu.
	 * Zwraca warto�� ujemn� w razie problemu z wyliczeniem warto�ci. */
	int getFieldIntensity(cv::Mat& grayImage, int x, int y);

	/** Je�li szachownica nie zosta�a znaleziona, nic nie zostanie narysowane.*/
	void drawGrid(cv::Mat& image, bool printCoords = true);
	void drawGrid(cv::Mat& image, int lineSize, cv::Scalar color);

	void drawCornersContours(cv::Mat& img) ;
	
protected:
	/**
	 * Po�o�enie kolejno lewego g�rnego, prawego g�rnego, lewego dolnego
	 * i prawego dolnego rogu szachownicy.
	 * W�a�ciwie jest to po�o�enie znalezionych znacznik�w. */
	std::vector<cv::Point2f> corners;

	bool cornersFound;
	cv::Size avgFieldSize;

	/** Punkty ko�cowe odcink�w rysowanych wewn�trznych linii szachownicy.
	 * Pierwszy wymiar: kraw�d� (g�ra/d� lub lewa/prawa), drugi: punkt. */
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