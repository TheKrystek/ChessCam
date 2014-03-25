#include "Pawn.h"

using namespace ChessCam;


Pawn::Pawn(Color color, Point pos, Chessboard& chessboard)
    : ChessPiece(color, pos, chessboard)
	{

	}

Pawn::~Pawn(){

}

bool Pawn::capture(Point goal){
    int dir = -1;
    if (this->color == black)
        dir = 1;

    if (abs(this->position.X() - goal.X())  == 1 && this->position.Y() - goal.Y() == dir)
		return true;
	return false;
}

bool Pawn::checkMove(Point goal){
    int dir = -1;
    if (this->color == black)
        dir = 1;
    // Jezeli nie wykonano ruchu to może ruszyc się o dwa pola
    if (this->numberOfMoves == 0)
        dir *=2;

    if (this->position.X() == goal.X() &&  this->position.Y()-goal.Y() == dir)
		return true;

    return false;
}


