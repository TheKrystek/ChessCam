#include "Queen.h"

using namespace ChessCam;


Queen::Queen(Color color, Point pos, Chessboard& chessboard) :
        ChessPiece(color, pos, chessboard) {

}

Queen::~Queen() {
//
}

bool Queen::capture(Point goal) {
    int step_x, step_y;
    if (abs(goal.X() - this->position.X()) == abs(goal.Y() - this->position.Y())) {
        step_x = ((goal.X() - position.X()) > 0 ? 1 : -1);
        step_y = ((goal.Y() - position.Y()) > 0 ? 1 : -1);
    }
    if (abs(goal.X() - this->position.X()) == 0 || abs(goal.Y() - this->position.Y()) == 0 ) {
        step_x = ((goal.X() - position.X()) == 0 ? 0 : ((goal.X() - position.X()) > 0 ? 1 : -1));
        step_y = ((goal.Y() - position.Y()) == 0 ? 0 : ((goal.X() - position.X()) > 0 ? 1 : -1));
    }

    if ((abs(goal.X() - this->position.X()) == 0 || abs(goal.Y() - this->position.Y()) == 0 ) ||
        (abs(goal.X() - this->position.X()) == abs(goal.Y() - this->position.Y()))){
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

bool Queen::checkMove(Point goal) {
    int step_x, step_y;
    if (abs(goal.X() - this->position.X()) == abs(goal.Y() - this->position.Y())) {
        step_x = ((goal.X() - position.X()) > 0 ? 1 : -1);
        step_y = ((goal.Y() - position.Y()) > 0 ? 1 : -1);
    }
    if (abs(goal.X() - this->position.X()) == 0 || abs(goal.Y() - this->position.Y()) == 0 ) {
        step_x = ((goal.X() - position.X()) == 0 ? 0 : ((goal.X() - position.X()) > 0 ? 1 : -1));
        step_y = ((goal.Y() - position.Y()) == 0 ? 0 : ((goal.X() - position.X()) > 0 ? 1 : -1));
    }

    if ((abs(goal.X() - this->position.X()) == 0 || abs(goal.Y() - this->position.Y()) == 0 ) ||
        (abs(goal.X() - this->position.X()) == abs(goal.Y() - this->position.Y()))){
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

