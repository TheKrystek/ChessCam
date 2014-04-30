#ifndef MOVE_RECOGNIZER_H
#define MOVE_RECOGNIZER_H

#include "ChessboardDetection.h"



/**
 * Lekka klasa kt�rej zadaniem jest rozpoznanie jaki ruch zosta� wykonany (o ile
 * zosta� wykonany) na podstawie przekazanych zmian na szachownicy.
 */
class MoveRecognizer
{
public:

	/**
	 * Typ wykonanego ruchu (lub b��d, je�li zmiany na szachownicy nie wskazuj�
	 * na poprawny ruch. */
	enum MoveType {
		INCORRECT,		/// niepoprawny lub nierozpoznany ruch lub braku ruchu
		MOVE,			/// przesuni�cie figury na wolne pole
		CAPTURING,		/// bicie
		CASTLING,		/// roszada
		EN_PASSANT,		/// bicie w przelocie
		NO_MOVE			/// nie wykonano �adnego ruchu (stan si� nie zmieni�)
	};
	
	/**
	 * Struktura przechowuj�ca informacje o zmianie stanu pola szachownicy. */
	struct ChangeInfo {
		cv::Point2i coords;		/// wsp�rz�dne w uk�adzie szachownicy <0;7>
		char colorLastState;	/// kolor figury w starym stanie (w/b/0)
		char colorNewState;		/// wykryty kolor figury w nowym satnie (w/b/0)
	};
	

	/**
	 * Sprawdza jaki PRAWDOPODOBNIE zosta� wykonany ruch.
	 * Na tym etapie nie jest mo�liwe sprawdzenie czy to rzeczywi�cie ten ruch,
	 * tym zajmuje sie modu� logiczny. Tutaj sprawdza, czy liczba i kolory
	 * pionk�w odpowiadaj� konkretnemu ruchowi.
	 * Do wektora moveCoords zostan� wpisane zmiany zgodnie z opisem
	 * metody getMoveCoords() w klasie FiguresFinder. */
	MoveType recognize(std::vector<ChangeInfo>& changes, bool whiteFiguresMove, std::vector<cv::Point2i>& moveCoords);

private:
	// zmiany kolejno dla: koloru wykonuj�cego ruch, drugiego koloru, pustego pola
	std::vector<ChangeInfo> orderedChanges[3];
	char movedFigureColor, secondFigureColor, emptyField;
	std::vector<cv::Point2i>* moveCoords;

	/** Sprawdzenie czy ruch jest przesuni�ciem figury na wolne pole. */
	bool isMove();
	/** Sprawdzenie czy ruch jest biciem (ale nie w przelocie). */
	bool isCapturing();
	/** Sprawdzenie czy ruch jest roszad�. */
	bool isCastling();
	/** Sprawdzenie czy ruch jest biciem w przelocie. */
	bool isEnPassant();

	enum FigureColors {
		MOVED_FIGURE_COLOR = 0,
		SECOND_FIGURE_COLOR = 1,
		EMPTY_FIELD = 2
	};

};


#endif // MOVE_RECOGNIZER_H