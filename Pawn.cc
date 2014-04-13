#include "Pawn.h"

using namespace ChessCam;


Pawn::Pawn(Color color, Point pos, Chessboard& chessboard)
    : ChessPiece(color, pos, chessboard)
	{
        this->enpassant = 0;
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

    // Jezeli pion moze wykonac bicie w ruchu
    if (enpassant){
        if (abs(this->position.X() - goal.X())  == 1 && this->position.Y() - goal.Y() == dir)
            if (!this->chessboard.isFree(Point(goal.X(),goal.Y() + dir)))
                if (enpassant == (long int)(this->chessboard.board[goal.X()][goal.Y() + dir]))
                {
                    enpassant = 0;
                    this->chessboard.capture(position,Point(goal.X(),goal.Y() + dir));
                    this->chessboard.move(Point(goal.X(),goal.Y() + dir),goal);
                    return true;
                }
    }

    // Jezeli nie wykonano ruchu to może ruszyc się o dwa pola
    if (this->numberOfMoves == 0)
        if (this->position.X() == goal.X() &&  this->position.Y()-goal.Y() == 2*dir){
            // Pion rusza sie o dwa pola, więc sprawdz czy istnieje pion który moze go zbic w przelocie, jezeli tak, to oznacz go

            int posY =  goal.Y();
            int posX[2] = {this->position.X() -1, this->position.X() + 1};

            for (int i=0;i<2;i++){
                if (posX[i] < 8 && posX[i] > -1)
                if (!this->chessboard.isFree(Point(posX[i],posY))){
                    Pawn *pion = dynamic_cast<Pawn*>(this->chessboard.board[posX[i]][posY]);
                    if (pion != NULL)
                        pion->enpassant = (long int)this;
                }
            }

            return true;
        }

    if (this->position.X() == goal.X() &&  this->position.Y()-goal.Y() == dir)
		return true;

    return false;
}


