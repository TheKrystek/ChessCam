#ifndef CHESSBOARDSTATE_H
#define CHESSBOARDSTATE_H

#include "ChessboardDetection.h"

typedef char State_t[8];


/**
 * Reprezentacja stanu szachownicy.
 */
class ChessboardState
{
public:

	ChessboardState();
	ChessboardState(ChessboardState& fromState);
	ChessboardState(State_t* state);

	char getField(int x, int y)						{ return state[y][x]; }
	State_t* getState()								{ return state; }

	void setField(int x, int y, char val)			{ state[y][x] = val; }
	
	/**
	 * Dokonuje porównania stanów. Zwraca true, jeœli s¹ takie same. */
	bool compare(ChessboardState& cmpState)			{ return compare(*this, cmpState); }

	void clear()									{ memset(state, 0, sizeof(state)); }


	static bool compare(ChessboardState& state1, ChessboardState& state2);
	static bool compare(State_t* state1, State_t* state2);

private:
	// wiersze, potem kolumny
	char state [8][8];

};


#endif // CHESSBOARDSTATE_H