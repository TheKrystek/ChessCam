#ifndef CHESSBOARD_OBSERVER_H
#define CHESSBOARD_OBSERVER_H
#include "opencv2/opencv.hpp"
#include "ChessboardDetector.h"
#include "FiguresFinder.h"

/**
 * G³ówna klasa modu³u wykrywania stanu szachownicy.
 */
class ChessboardObserver
{
public:
    ChessboardObserver();
	~ChessboardObserver();


	/** Rozpocznij przechwytywanie obrazu i rozpoznawanie stanu szachownicy. */
	void start();
    void start(cv::VideoCapture );
	/** Rozpoznaj aktualny stan szachownicy. */
	DetectionState checkState();

	/** Cofnij poprzedni ruch. Przywraca poprzedni stan szachownicy. */
	bool undoneMove();

	/** Pobierz informacje o ostatnim ruchu. Patrz: FiguresFinder.getLastMove(). */
	void getLastMove(std::vector<cv::Point2i>& moveCoords);


    void drawResults();
cv::Mat* getFrame();
protected:
	cv::VideoCapture cap;
	cv::Mat frame;

	ChessboardDetector chessboardDetector;
	FiguresFinder figuresFinder;

    void estimateContrast(void* state, cv::Mat& grayImage);
};

#endif // CHESSBOARD_OBSERVER_H
