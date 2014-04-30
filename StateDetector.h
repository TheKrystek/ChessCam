#ifndef STATE_DETECTOR_H
#define STATE_DETECTOR_H

#include "ChessboardDetection.h"
#include "ChessboardDetector.h"
#include "ChessboardState.h"


/**
 * Klasa sprawdzaj¹ca aktualny stan szachownicy (rozmieszczenie figur).
 */
class StateDetector
{
public:

	StateDetector(ChessboardDetector* chessboardDetector, int frames = 7, int threshold = 4);
	~StateDetector();

	/**
	 * Podejmuje próbê znalezienia figur w danej klatce. Zwraca true,
	 * jeœli rozpoznawanie siê zakoñczy, tj. przerobiona zostanie liczba
	 * klatek równa parametrowi frames. */
	bool detect(cv::Mat* frame);

	/**
	 * Czy wykrywanie wci¹¿ trwa, tj. zosta³o rozpoczête i nie zosta³o zakoñczone. */
	bool isDetecting()							{ return detecting; }

	/**
	 * Przerywa wykrywanie. Ustawia stan skaluj¹c próg wzglêdem rozpatrzonej
	 * liczby klatek. */
	void stop();

	/**
	 * Zwraca wykryty stan figur (tablica char[8][8], wiersze i kolumny szachownicy).
	 * Jeœli wykrywanie nie zosta³o rozpoczête ani zakoñczone lub przerwane,
	 * tablica bêdzie wyzerowana. */
	State_t* getState()							{ return state; }

	cv::Mat& getLastFrameAsGray()				{ return gray; }

	void setChessboardDetector(ChessboardDetector* chessboardDetector);
	void setFrames(int frames)					{ this->frames = frames; }
	void setThreshold(int threshold)			{ this->threshold = threshold; }

	void setContrast(int contrast)				{ this->contrast = contrast; }
	void setWhiteFigureThreshold(int threshold)	{ this->whiteFigureThreshold = threshold; }

	/**
	 * Wykryj po³o¿enie figur bez miany wewnêtrznego stanu obiektu detektora.
	 * Dla podgl¹du dzia³ania lub kalibracji. */
	void findFigures(cv::Mat& frame);

	/**
	 * Tworzy obraz w odcieniu szaroœci z u¿yciem aktualnej wartoœci kontrastu.
	 * Potrzebne do automatycznego ustawiania kontrastu.
	 * Jeœli parametr grayFrame == NULL, to ustawia wewnêrzne pole. */
	void prepareGrayFrame(cv::Mat* frame, cv::Mat* grayFrame = NULL);

private:
	int frames;
	int threshold;
	ChessboardDetector* chessboardDetector;
	int contrast;
	int whiteFigureThreshold;

	int currentFrameNum;
	bool detecting;
	cv::Mat* currentFrame;
	char state[8][8];
	char counter[8][8];
	cv::Mat gray;

	void findFigures(cv::Mat& frame, cv::Mat& gray, char counter[8][8]);

	/**
	 * Okreœl kolory wykrytych figur. Dzia³a na tablicy state. */
	void determineColors();

	void createTrackbars();
};


#endif // STATE_DETECTOR_H