#ifndef PAWN_H
#define PAWN_H
#include "ChessPiece.h"


namespace ChessCam {
class Pawn: public ChessPiece
{

public:

	Pawn(Color, Point, Chessboard&);
	~Pawn();
	bool checkMove(Point);
	bool capture(Point);
    long int enpassant; // czy może wykonać bicie en passant (w przelocie),
};

}

#endif // PAWN_H

#ifndef CHESSPIECE_H
#define CHESSPIECE_H
#endif
