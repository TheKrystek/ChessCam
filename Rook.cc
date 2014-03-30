#include "Rook.h"

using namespace ChessCam;


Rook::Rook(Color color, Point pos, Chessboard& chessboard) :
		ChessPiece(color, pos, chessboard) {
        this->castling = false;
}

Rook::~Rook() {
//
}

bool Rook::capture(Point goal) {
    if (abs(goal.X() - this->position.X()) == 0 || abs(goal.Y() - this->position.Y()) == 0 ) {
        int step_x = ((goal.X() - position.X()) == 0 ? 0 : ((goal.X() - position.X()) > 0 ? 1 : -1));
        int step_y = ((goal.Y() - position.Y()) == 0 ? 0 : ((goal.Y() - position.Y()) > 0 ? 1 : -1));
        goal.setX(goal.X() - step_x);
        goal.setY(goal.Y() - step_y);
        while(goal.X() != position.X() && goal.Y() != position.Y()){
            if (!this->chessboard.isFree(goal))
                return false;
            goal.setX(goal.X() - step_x);
            goal.setY(goal.Y() - step_y);
        }
        return true;
    }
    return false;
}

bool Rook::checkMove(Point goal) {

    // Sprawdz czy nie wykonuje roszady
    if (this->numberOfMoves == 0)
        if (goal.X() == 3 || goal.X() == 5)
            this->castling = true;  // oznacz, ze moze wykonywac roszadę

    if (abs(goal.X() - this->position.X()) == 0 || abs(goal.Y() - this->position.Y()) == 0 ) {
        int step_x = ((goal.X() - position.X()) == 0 ? 0 : ((goal.X() - position.X()) > 0 ? 1 : -1));
        int step_y = ((goal.Y() - position.Y()) == 0 ? 0 : ((goal.Y() - position.Y()) > 0 ? 1 : -1));
        while(goal.X() != position.X() && goal.Y() != position.Y()){
            if (!this->chessboard.isFree(goal))
                return false;
            goal.setX(goal.X() - step_x);
            goal.setY(goal.Y() - step_y);
        }
        return true;
    }
	return false;
}

