#include "Bishop.h"

using namespace ChessCam;


Bishop::Bishop(Color color, Point pos, Chessboard& chessboard) :
        ChessPiece(color, pos, chessboard)
 {}

Bishop::~Bishop() {
//
}

bool Bishop::capture(Point goal) {
    if (abs(goal.X() - this->position.X()) == abs(goal.Y() - this->position.Y())) {
        int step_x = ((goal.X() - position.X()) > 0 ? 1 : -1);
        int step_y = ((goal.Y() - position.Y()) > 0 ? 1 : -1);
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

bool Bishop::checkMove(Point goal) {
    if (abs(goal.X() - this->position.X()) == abs(goal.Y() - this->position.Y())) {
        int step_x = ((goal.X() - position.X()) > 0 ? 1 : -1);
        int step_y = ((goal.Y() - position.Y()) > 0 ? 1 : -1);
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
