#ifndef STATE_DETECTOR_H
#define STATE_DETECTOR_H

#include "ChessboardDetection.h"
#include "ChessboardDetector.h"
#include "ChessboardState.h"


/**
 * Klasa sprawdzaj�ca aktualny stan szachownicy (rozmieszczenie figur).
 */
class StateDetector
{
public:

	StateDetector(ChessboardDetector* chessboardDetector, int frames = 7, int threshold = 4);
	~StateDetector();

	/**
	 * Podejmuje pr�b� znalezienia figur w danej klatce. Zwraca true,
	 * je�li rozpoznawanie si� zako�czy, tj. przerobiona zostanie liczba
	 * klatek r�wna parametrowi frames. */
	bool detect(cv::Mat* frame);

	/**
	 * Czy wykrywanie wci�� trwa, tj. zosta�o rozpocz�te i nie zosta�o zako�czone. */
	bool isDetecting()							{ return detecting; }

	/**
	 * Przerywa wykrywanie. Ustawia stan skaluj�c pr�g wzgl�dem rozpatrzonej
	 * liczby klatek. */
	void stop();

	/**
	 * Zwraca wykryty stan figur (tablica char[8][8], wiersze i kolumny szachownicy).
	 * Je�li wykrywanie nie zosta�o rozpocz�te ani zako�czone lub przerwane,
	 * tablica b�dzie wyzerowana. */
	State_t* getState()							{ return state; }

	cv::Mat& getLastFrameAsGray()				{ return gray; }

	void setChessboardDetector(ChessboardDetector* chessboardDetector);
	void setFrames(int frames)					{ this->frames = frames; }
	void setThreshold(int threshold)			{ this->threshold = threshold; }

	void setContrast(int contrast)				{ this->contrast = contrast; }
	void setWhiteFigureThreshold(int threshold)	{ this->whiteFigureThreshold = threshold; }

	/**
	 * Wykryj po�o�enie figur bez miany wewn�trznego stanu obiektu detektora.
	 * Dla podgl�du dzia�ania lub kalibracji. */
	void findFigures(cv::Mat& frame);

	/**
	 * Tworzy obraz w odcieniu szaro�ci z u�yciem aktualnej warto�ci kontrastu.
	 * Potrzebne do automatycznego ustawiania kontrastu.
	 * Je�li parametr grayFrame == NULL, to ustawia wewn�rzne pole. */
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
	 * Okre�l kolory wykrytych figur. Dzia�a na tablicy state. */
	void determineColors();

	void createTrackbars();
};


#endif // STATE_DETECTOR_H