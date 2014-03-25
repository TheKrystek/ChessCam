#ifndef KNIGHT_H
#define KNIGHT_H
#include "ChessPiece.h"


namespace ChessCam {
class Knight: public ChessPiece
{
public:
	Knight(Color, Point, Chessboard&);
	~Knight();
	bool checkMove(Point);
	bool capture(Point);
};
}


#endif // KNIGHT_H

#ifndef CHESSPIECE_H
#define CHESSPIECE_H
#endif
