#include "Chessboard.h"
#include "Pawn.h"
#include "Rook.h"
#include "Bishop.h"
#include "King.h"
#include "Knight.h"
#include "Queen.h"
#include <iostream>
#include <cstddef>
#include <QString>

using namespace std;
using namespace ChessCam;

Chessboard::Chessboard() {

	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
            this->board[i][j] = NULL;

	this->setPieces();
    this->whiteMoves = true;
}

void Chessboard::setPieces() {
	Color color = white;
	int firstRow = 0;
	int secondRow = 1;

	// Wykonaj to samo dla białych i czarnych
	for (int i = 0; i < 2; i++) {
		color = (i == 0 ? white : black);
		firstRow = (i == 0 ? 0 : 7);
		secondRow = (i == 0 ? 1 : 6);

		// Ustaw pionki
		for (int j = 0; j < 8; j++)
            this->board[j][secondRow] = new Pawn(color, Point(j, secondRow), *this);

		// Ustaw wieze
        this->board[0][firstRow] = new Rook(color, Point(0, firstRow), *this);
        this->board[7][firstRow] = new Rook(color, Point(7, firstRow), *this);
		// Ustaw konie/skoczki
        this->board[1][firstRow] = new Knight(color, Point(1, firstRow), *this);
        this->board[6][firstRow] = new Knight(color, Point(6, firstRow), *this);

        // Ustaw gońców
        this->board[2][firstRow] = new Bishop(color, Point(2, firstRow), *this);
        this->board[5][firstRow] = new Bishop(color, Point(5, firstRow), *this);

		// Ustaw króla i królową
        this->board[3][firstRow] = new Queen(color, Point(3, firstRow), *this);
        this->board[4][firstRow] = new King(color, Point(4, firstRow), *this);

	}
}


bool Chessboard::tryToMove(Point start, Point end) {
    if (!this->isFree(start)) {
        bool moved = false;
        // Sprawdz czy krol nie wykonuje ruchu po raz pierwsz,
        // bo moze wieza zaczela wykonywac roszade
        if (this->getPieceType(start) == "king")
        {
            // Sprawdz krotka roszade
            if (!this->isFree(Point(5,start.Y())))
                if (this->board[5][start.Y()]->castling)
                    this->board[start.X()][start.Y()]->castling = true; // Oznacz ze moze wykonac roszade

            // Sprawdz dluga roszade
            if (!this->isFree(Point(3,start.Y())))
                if (this->board[3][start.Y()]->castling)
                    this->board[start.X()][start.Y()]->castling = true; // Oznacz ze moze wykonac roszade

            // Wykonaj ruch roszady krolem
            if (this->board[start.X()][start.Y()]->castling)
                moved = this->board[start.X()][start.Y()]->moveTo(end);
            if (moved)
                return true;
        }
        // Jezeli nie rusza sie krol, to wyczysc mozliwosc roszady przeciwnikowi
        else{
            for (int i=0; i<8;i++)
                for (int j=0;j<8;j++)
                    if (!this->isFree(Point(i,j)) && (this->getColor(Point(i,j)) == black) == this->whiteMoves)
                        this->board[i][j]->castling = false;
        }

		// Sprawdz czy dobra kolejnosc ruchow
        if (this->whiteMoves == (this->getColor(start) == white)) {
            moved = this->board[start.X()][start.Y()]->moveTo(end);
			if (moved)
				this->whiteMoves = !this->whiteMoves;
		} else
            if (!this->whiteMoves)
                throw "Teraz ruch czarnych";
            else
                throw "Teraz ruch białych";
		return moved;
	}
    throw "Brak pionka";
    return false;
}

// przenies figure z x1; y1 do x2; y2
bool Chessboard::move(Point start, Point end) {
    if (this->isFree(end)) {
        this->board[end.X()][end.Y()] = this->board[start.X()][start.Y()];
        this->board[start.X()][start.Y()] = NULL;
        emit moved(start,end);
	}
	return false;
}

// figura o wsp x1; y1 zajmij miejsce fig x2; y2
bool Chessboard::capture(Point start, Point end) {
    if (!this->isFree(end)) {

        if (this->getColor(end) == white)
            this->whiteCaptured.push_back(getPieceType(end));
        else
            this->blackCaptured.push_back(getPieceType(end));

        delete this->board[end.X()][end.Y()];
        this->board[end.X()][end.Y()] = this->board[start.X()][start.Y()];
        this->board[start.X()][start.Y()] = NULL;
        emit captured(start,end);
	}
	return false;
}

void Chessboard::deletePiece(Point goal) {
    if (!this->isFree(goal)) {
        delete this->board[goal.X()][goal.Y()];
        this->board[goal.X()][goal.Y()] = NULL;
	}
}

const char* Chessboard::getPieceType(Point goal){
    if (dynamic_cast<Bishop*>(board[goal.X()][goal.Y()]))
        return "bishop";
    if (dynamic_cast<King*>(board[goal.X()][goal.Y()]))
        return "king";
    if (dynamic_cast<Knight*>(board[goal.X()][goal.Y()]))
        return "knight";
    if (dynamic_cast<Pawn*>(board[goal.X()][goal.Y()]))
        return "pawn";
    if (dynamic_cast<Queen*>(board[goal.X()][goal.Y()]))
        return "queen";
    if (dynamic_cast<Rook*>(board[goal.X()][goal.Y()]))
        return "rook";
    return "pawn";
}

Color Chessboard::getColor(Point goal) {
    if (!this->isFree(goal))
        return this->board[goal.X()][goal.Y()]->getColor();
    throw "NULL exception w getColor";
}

bool Chessboard::isFree(Point goal) {
    return (this->board[goal.X()][goal.Y()] == NULL);
}

bool Chessboard::isMoving(Point goal) {
    if (!this->isFree(goal))
        return this->board[goal.X()][goal.Y()]->moving();
    return true;
}

void Chessboard::toggleMoving(Point goal) {
    if (!this->isFree(goal))
        this->board[goal.X()][goal.Y()]->toggleMoving();
}

bool Chessboard::check(){
    Color color = black;
    if (whiteMoves)
        color = white;

    int king_x, king_y;
    // Znajdz wsp wlasnego krola
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if (!this->isFree(Point(i,j)))
                if (this->board[i][j]->getColor() == color && this->getPieceType(Point(i,j)) == "king")
                {
                    king_y = j;
                    king_x = i;
                }

    // Sprawdz czy przeciwnik dysponuje ruchem szachujacym
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if (!this->isFree(Point(i,j)))
                if (this->board[i][j]->getColor() != color)
                    if (this->board[i][j]->capture(Point(king_x,king_y)))
                        return true;

    return false;
}

Chessboard::~Chessboard(){

    // Usun wszystkie pionki
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if (!this->isFree(Point(i,j)))
                delete this->board[i][j];

    // Opróżnij wektory ze zbitymi pionkami
    blackCaptured.clear();
    whiteCaptured.clear();
}
