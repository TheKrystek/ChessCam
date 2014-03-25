#include "ChessPiece.h"
#include <iostream>

using namespace ChessCam;


ChessPiece::ChessPiece(Color color, Point pos, Chessboard& cb) :
		chessboard(cb) {
	this->color = color;
    this->position = pos;
	this->numberOfMoves = 0;
    this->isMoving = false;

}

void ChessPiece::displayPosition() {
	std::cout << " [";
	std::cout << (color == white ? "w" : "b");
	std::cout << "] (";
    std::cout << this->position.X();
    std::cout << "," << this->position.Y() << ")";
}

bool ChessPiece::moveTo(Point goal) {
	// Sprawdz czy pole na ktore chcemy sie ruszyc jest wolne
    if (this->chessboard.isFree(goal)) {
		//Sprawdz czy mozemy wykonac taki ruch
        if (this->checkMove(goal)) {
            this->chessboard.move(this->position, goal);
            position = goal;
			this->numberOfMoves++;
			return true;
		} else
			throw "Niepoprawny ruch 1";
	} else {
		// Jezeli pole jest zajete to sprawdz czy mozna je bic
		// Czyli sprawdz czy pole jest zejete przez pionek innego koloru
		// i czy w ogole mozna bic tym pionkiem w taki sposób
        if (this->chessboard.getColor(goal) != this->getColor() && this->capture(goal)) {
            this->chessboard.capture(position,goal);
            position = goal;
            this->numberOfMoves++;
			return true;
		}
		else
			throw "Niepoprawny ruch 2";
	}
	return false;
}

Color ChessPiece::getColor() {
	return this->color;
}

bool ChessPiece::moving(){
    return this->isMoving;
}

void ChessPiece::toggleMoving(){
    this->isMoving = !this->isMoving;
}

ChessPiece::~ChessPiece() {

}

