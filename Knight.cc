#include "Knight.h"

using namespace ChessCam;


Knight::Knight(Color color, Point pos, Chessboard& chessboard)
	: ChessPiece(color, pos, chessboard)
	{

	}

Knight::~Knight(){
//
}

bool Knight::capture(Point goal){
    return this->checkMove(goal);
}

bool Knight::checkMove(Point goal){
    if ((abs(goal.X() - this->position.X()) == 1 && abs(goal.Y() - this->position.Y()) == 2) ||
        (abs(goal.X() - this->position.X()) == 2 && abs(goal.Y() - this->position.Y()) == 1))
		return true;
	return false;
}


