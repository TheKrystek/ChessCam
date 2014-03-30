#ifndef KING_H
#define KING_H
#include "ChessPiece.h"


namespace ChessCam {
class King: public ChessPiece
{
public:
	King(Color, Point, Chessboard&);
	~King();
	bool checkMove(Point);
    bool capture(Point);
};
}


#endif // KING_H

#ifndef CHESSPIECE_H
#define CHESSPIECE_H
#endif
