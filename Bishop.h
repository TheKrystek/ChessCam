#ifndef BISHOP_H
#define BISHOP_H
#include "ChessPiece.h"


namespace ChessCam {
class Bishop: public ChessPiece
{
public:
    Bishop(Color, Point, Chessboard&);
	~Bishop();
    bool checkMove(Point);
    bool capture(Point);
};
}


#endif // BISHOP_H

#ifndef CHESSPIECE_H
#define CHESSPIECE_H
#endif
