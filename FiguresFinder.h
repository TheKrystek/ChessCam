#ifndef FIGURES_FINDER_H
#define FIGURES_FINDER_H

#include "ChessboardDetection.h"
#include "ChessboardDetector.h"
#include "StateDetector.h"
#include "ChessboardState.h"

#include <deque>


/**
 * Klasa pozwalaj�ca wykrywa� ruch na szachownicy.
 */
class FiguresFinder
{
public:
	FiguresFinder(ChessboardDetector* chessboardDetector = NULL);
	~FiguresFinder();

	/**
	 * Wykonaj sprawdzanie ruchu figur.
	 * Najpierw dokonuje detekcji ruchu r�ki, nast�pnie pr�buje ustali� stan
	 * ruchu, ostatecznie ustalaj�c stan zaj�to�ci p�l szachownicy. */
	DetectionState perform(cv::Mat* frame);

	/**
	 * Sprawdzenie stanu szachownicy dla kalibracji. Nie zmienia wewn�trznego
	 * stanu szachownicy. */
	void performOnCalibrating(cv::Mat* frame);

	/**
	 * Zwraca wsp�rz�dne kolejno pola szachownicy z kt�rego wykonano
	 * ostatni ruch oraz pola na kt�rym figura si� zatrzyma�a.
	 * Mo�e zawiera� 2 lub 4 elementy - drugi przypadek je�li nast�pi roszada.
	 * Wsp�rzedne x,y z przedzia�u <0;7>.
	 * W przypadku roszady: pierwsza para wsp�rz�dnych to ruch kr�la.*/
	void getLastMove(std::vector<cv::Point2i>& moveCoords);

	/**
	 * Wywo�ywane po pierwszej kalibracji. Ustawia pocz�tkowe ustawienie figur. */
	void start();

	/*
	 * Przywraca poprzedni stan szachownicy (czyli: cofa wykonany ruch).
	 * Zwraca false je�li nie uda si� cofn�� stanu (np. brak zapami�tanych). */
	bool undoneMove();

	/**
	 * Zaznacz zaj�to�� p�l. ChessboardDetector potrzebny jest do zamiany
	 * uk�ad�w wsp�rz�dnych.
	 * Gdy w��czona jest kalibracja, nie rysuje w�a�ciwego stanu, tylko
	 * wykrywany stan podczas kalibracji. */
	void drawFigures(cv::Mat& image);
	
	/** Czy aktualnie trwa ruch gracza? */
	bool getIsPlayerMove() const					{ return isPlayerMove; }

	/** Czy nad szachownic� wykryto r�k� w ostatniej ramce? */
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

	/** Ostatni, aktualny oraz nowy (w trakcie przetwarzania) stan zaj�to�ci p�l. */
	char lastState[8][8];
	char currentState[8][8];
	char newState[8][8];

	/** Informacje dla kalibracji wykrywania szach�w. */
	char calibrationState[8][8];
	StateDetector* calibrationStateDetector;

	/** Czy poprzedni wykryty stan by� poprawny? (Na tyle, na ile mo�na na tym
	 * etapie wykry�). */
	bool isStateOK;		

	/** Czy rozpoznany poprawny ruch figury? */
	bool isMoveRecognized;


	/** Wsp�rz�dne szachownicy: pole z kt�rego figura rozpocz�a ruch oraz
	 * pole na kt�rym stan�a. Wsp�rz�dne z przedzia�u <0;7>. 
	 * Wektor, poniewa� w przypadku roszady mamy ruch dw�ch figur. */
	std::vector<cv::Point2i> moveCoords;

	cv::Mat* lastFrame;

	ChessboardDetector* chessboardDetector;
	StateDetector* stateDetector;

	std::deque<ChessboardState*> statesHistory;

	
	/**
	 * Sprawd� czy nad szachownic� znajduje si� r�ka gracza. Wewn�rzny stan
	 * zostanie uakualniony. */
	bool detectHand();

	/**
	 * Wyznacz nowy stan po�o�enia figur. Je�li si� uda (isStateOK == OK),
	 * zmodyfikuje currentState. */
	void determineNewState();
	
	/**
	 * Wykryj jaki wykonano ruch (o ile wykonano). Por�wnywany jest nowy stan
	 * szachownicy ze starym. Zwraca true je�li wykryto zmian� i jest to
	 * poprawny ruch (na tyle, na ile mo�na tutaj stwierdzi�).
	 * Wtedy ustawione zostan� wewn�trzne pola pami�aj�ce sk�d i dok�d
	 * przesuni�to figur�.
	 * Je�li wykryje wiele zmian lub niepoprawne zmiany, zwr�ci false.
	 * Ustawia tak�e pole isStateOK aby zaznaczy� czy wykryty stan jest dobry.*/
	bool detectFigureMove();
};


#endif // FIGURES_FINDER_H