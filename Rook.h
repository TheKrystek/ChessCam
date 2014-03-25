#ifndef ROOK_H
#define ROOK_H
#include "ChessPiece.h"


namespace ChessCam {
class Rook: public ChessPiece
{
public:
	Rook(Color, Point, Chessboard&);
	~Rook();
	bool checkMove(Point);
	bool capture(Point);
};
}


#endif // ROOK_H

#ifndef CHESSPIECE_H
#define CHESSPIECE_H
#endif
