#ifndef CHESSBOARD_H
#define CHESSBOARD_H
#include "Color.h"
#include <stddef.h>
#include <iostream>
#include <vector>
#include "Point.h"
#include <QObject>

namespace ChessCam {

class ChessPiece;

class Chessboard  : public QObject{
    Q_OBJECT
    friend class ChessPiece;
    friend class Pawn;

private:
    bool move(Point, Point);        // Przesun pionek
    ChessPiece * board[8][8];       // Macierz wskaznikow na pionki - 8x8
    void setPieces();
public:
    Chessboard();
    ~Chessboard();

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
    void toggleMoving(Point);


signals:
    void moved(Point,Point);
    void captured(Point,Point);

};

}
#endif
