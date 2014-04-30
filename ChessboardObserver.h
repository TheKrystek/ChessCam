#ifndef CHESSBOARD_OBSERVER_H
#define CHESSBOARD_OBSERVER_H

#include "ChessboardDetector.h"
#include "FiguresFinder.h"

/**
 * G³ówna klasa modu³u wykrywania stanu szachownicy.
 */
class ChessboardObserver
{
public:
	ChessboardObserver(cv::VideoCapture cap);
	~ChessboardObserver();


	/** Rozpocznij przechwytywanie obrazu i rozpoznawanie stanu szachownicy. */
	void start();

	/** Rozpoznaj aktualny stan szachownicy. */
	DetectionState checkState();

	/** Cofnij poprzedni ruch. Przywraca poprzedni stan szachownicy. */
	bool undoneMove();

	/** Pobierz informacje o ostatnim ruchu. Patrz: FiguresFinder.getLastMove(). */
	void getLastMove(std::vector<cv::Point2i>& moveCoords);


protected:
	cv::VideoCapture cap;
	cv::Mat frame;

	ChessboardDetector chessboardDetector;
	FiguresFinder figuresFinder;

	void drawResults();
	void estimateContrast(void* state, cv::Mat& grayImage);
};

#endif // CHESSBOARD_OBSERVER_H