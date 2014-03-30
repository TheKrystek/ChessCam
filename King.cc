#include "King.h"

using namespace ChessCam;


King::King(Color color, Point pos, Chessboard& chessboard)
	: ChessPiece(color, pos, chessboard)
	{
        this->castling = false;
	}

King::~King(){
//
}

bool King::capture(Point goal){
    return this->checkMove(goal);
}

bool King::checkMove(Point goal){
     if (this->castling){
        // Jezeli krol sie jeszcze nie ruszyl i moze wykonac roszade
        if (this->numberOfMoves == 0)
            return true;
        this->castling = false;
    }
    if (abs(goal.X() - this->position.X()) == 1 || abs(goal.Y() - this->position.Y()) == 1)
		return true;
	return false;
}


