#ifndef QUEEN_H
#define QUEEN_H
#include "ChessPiece.h"


namespace ChessCam {
class Queen: public ChessPiece
{
public:
    Queen(Color, Point, Chessboard&);
	~Queen();
    bool checkMove(Point);
    bool capture(Point);
};
}


#endif // QUEEN_H

#ifndef CHESSPIECE_H
#define CHESSPIECE_H
#endif
