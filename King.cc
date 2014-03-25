#include "King.h"

using namespace ChessCam;


King::King(Color color, Point pos, Chessboard& chessboard)
	: ChessPiece(color, pos, chessboard)
	{

	}

King::~King(){
//
}

bool King::capture(Point goal){
    return this->checkMove(goal);
}

bool King::checkMove(Point goal){
    if (abs(goal.X() - this->position.X()) == 1 || abs(goal.Y() - this->position.Y()) == 1)
		return true;
	return false;
}


