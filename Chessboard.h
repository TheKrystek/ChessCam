#ifndef CHESSBOARD_H
#define CHESSBOARD_H
#include "Color.h"
#include <stddef.h>
#include <iostream>
#include <vector>
#include "Point.h"


namespace ChessCam {

class ChessPiece;

class Chessboard {
	friend class ChessPiece;

private:
    bool move(Point, Point);  // Przesun pionek
    ChessPiece *** board;           // Macierz wskaznikow na pionki - 8x8
    void setPieces();
public:
    bool whiteMoves;
    std::vector<const char*> whiteCaptured;
    std::vector<const char*> blackCaptured;

    bool tryToMove(Point, Point);
    bool capture(Point, Point);
    void deletePiece(Point);
    bool check();

    bool isFree(Point);
    bool isMoving(Point);
    Color getColor(Point);
    const char* getPieceType(Point);

	void displayPieces();
    void displayPiece(Point, bool);
    void toggleMoving(Point);

    Chessboard();
    ~Chessboard();
};

}
#endif
