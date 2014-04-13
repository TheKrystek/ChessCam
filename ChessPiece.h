#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include "Color.h"
#include "Chessboard.h"
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include "Point.h"



namespace ChessCam {

class ChessPiece
{

public:
    ChessPiece(Color, Point, Chessboard&);
	virtual ~ChessPiece();

    bool moveTo(Point);                 // Rusz się do miejsca
    virtual bool checkMove(Point) = 0;   // Sprawdz czy mozna wykonac takie bicie
    virtual bool capture(Point) = 0;    // Sprawdz czy mozna wykonac taki ruch

    bool moving();          // Sprawdz czy pionek sie rusza
    void toggleMoving();    // Przelacz on/off czy pionek sie rusza

    Color getColor();       // Zwroc kolor pionka
    bool castling;          // Roszada wykorzystywana jedynie przez wieze i krola
protected:
	Chessboard& chessboard;

    Point position;      // (X,Y) określaja polozenie pionka na szachownicy
    int numberOfMoves;  // Liczba wykonanych ruchów
    bool isMoving;      // Okresla, czy pionek aktualnie wykonuje ruch
    Color color;        // Kolor pionka




};

}
#endif //CHESSPIECE_H
