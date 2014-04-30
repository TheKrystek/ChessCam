#ifndef FIGURES_FINDER_H
#define FIGURES_FINDER_H

#include "ChessboardDetection.h"
#include "ChessboardDetector.h"
#include "StateDetector.h"
#include "ChessboardState.h"

#include <deque>


/**
 * Klasa pozwalaj¹ca wykrywaæ ruch na szachownicy.
 */
class FiguresFinder
{
public:
	FiguresFinder(ChessboardDetector* chessboardDetector = NULL);
	~FiguresFinder();

	/**
	 * Wykonaj sprawdzanie ruchu figur.
	 * Najpierw dokonuje detekcji ruchu rêki, nastêpnie próbuje ustaliæ stan
	 * ruchu, ostatecznie ustalaj¹c stan zajêtoœci pól szachownicy. */
	DetectionState perform(cv::Mat* frame);

	/**
	 * Sprawdzenie stanu szachownicy dla kalibracji. Nie zmienia wewnêtrznego
	 * stanu szachownicy. */
	void performOnCalibrating(cv::Mat* frame);

	/**
	 * Zwraca wspó³rzêdne kolejno pola szachownicy z którego wykonano
	 * ostatni ruch oraz pola na którym figura siê zatrzyma³a.
	 * Mo¿e zawieraæ 2 lub 4 elementy - drugi przypadek jeœli nast¹pi roszada.
	 * Wspó³rzedne x,y z przedzia³u <0;7>.
	 * W przypadku roszady: pierwsza para wspó³rzêdnych to ruch króla.*/
	void getLastMove(std::vector<cv::Point2i>& moveCoords);

	/**
	 * Wywo³ywane po pierwszej kalibracji. Ustawia pocz¹tkowe ustawienie figur. */
	void start();

	/*
	 * Przywraca poprzedni stan szachownicy (czyli: cofa wykonany ruch).
	 * Zwraca false jeœli nie uda siê cofn¹æ stanu (np. brak zapamiêtanych). */
	bool undoneMove();

	/**
	 * Zaznacz zajêtoœæ pól. ChessboardDetector potrzebny jest do zamiany
	 * uk³adów wspó³rzêdnych.
	 * Gdy w³¹czona jest kalibracja, nie rysuje w³aœciwego stanu, tylko
	 * wykrywany stan podczas kalibracji. */
	void drawFigures(cv::Mat& image);
	
	/** Czy aktualnie trwa ruch gracza? */
	bool getIsPlayerMove() const					{ return isPlayerMove; }

	/** Czy nad szachownic¹ wykryto rêkê w ostatniej ramce? */
	bool getIsHand() const							{ return isHand; }

	void setChessboardDetector(ChessboardDetector* chessboardDetector);

	StateDetector* getStateDetector()				{ return stateDetector; }
	StateDetector* getCalibrationStateDetector()	{ return calibrationStateDetector; }

	bool getIsStarted()								{ return isStarted; }
	State_t* getCurrentState()						{ return currentState; }
	bool getIsWhiteFiguresMove()					{ return isWhiteFiguresMove; }

	
protected:
	bool isPlayerMove;
	bool isHand;
	bool isWhiteFiguresMove;
	bool isStarted;

	/** Ostatni, aktualny oraz nowy (w trakcie przetwarzania) stan zajêtoœci pól. */
	char lastState[8][8];
	char currentState[8][8];
	char newState[8][8];

	/** Informacje dla kalibracji wykrywania szachów. */
	char calibrationState[8][8];
	StateDetector* calibrationStateDetector;

	/** Czy poprzedni wykryty stan by³ poprawny? (Na tyle, na ile mo¿na na tym
	 * etapie wykryæ). */
	bool isStateOK;		

	/** Czy rozpoznany poprawny ruch figury? */
	bool isMoveRecognized;


	/** Wspó³rzêdne szachownicy: pole z którego figura rozpoczê³a ruch oraz
	 * pole na którym stanê³a. Wspó³rzêdne z przedzia³u <0;7>. 
	 * Wektor, poniewa¿ w przypadku roszady mamy ruch dwóch figur. */
	std::vector<cv::Point2i> moveCoords;

	cv::Mat* lastFrame;

	ChessboardDetector* chessboardDetector;
	StateDetector* stateDetector;

	std::deque<ChessboardState*> statesHistory;

	
	/**
	 * SprawdŸ czy nad szachownic¹ znajduje siê rêka gracza. Wewnêrzny stan
	 * zostanie uakualniony. */
	bool detectHand();

	/**
	 * Wyznacz nowy stan po³o¿enia figur. Jeœli siê uda (isStateOK == OK),
	 * zmodyfikuje currentState. */
	void determineNewState();
	
	/**
	 * Wykryj jaki wykonano ruch (o ile wykonano). Porównywany jest nowy stan
	 * szachownicy ze starym. Zwraca true jeœli wykryto zmianê i jest to
	 * poprawny ruch (na tyle, na ile mo¿na tutaj stwierdziæ).
	 * Wtedy ustawione zostan¹ wewnêtrzne pola pamiêaj¹ce sk¹d i dok¹d
	 * przesuniêto figurê.
	 * Jeœli wykryje wiele zmian lub niepoprawne zmiany, zwróci false.
	 * Ustawia tak¿e pole isStateOK aby zaznaczyæ czy wykryty stan jest dobry.*/
	bool detectFigureMove();
};


#endif // FIGURES_FINDER_H