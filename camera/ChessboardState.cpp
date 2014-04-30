#include "ChessboardState.h"

//using namespace cv;


ChessboardState::ChessboardState() {
	memset(state, 0, sizeof(state));
}

ChessboardState::ChessboardState(ChessboardState& fromState) {
	memcpy(state, fromState.getState(), sizeof(state));
}

ChessboardState::ChessboardState(State_t* stateFrom) {
	memcpy(state, stateFrom, sizeof(state));
}

bool ChessboardState::compare(ChessboardState& state1, ChessboardState& state2) {
	return compare(state1.getState(), state2.getState());
}

bool ChessboardState::compare(State_t* state1, State_t* state2) {
	for (int y=0; y<8; y++)
		for (int x=0; x<8; x++)
			if (state1[y][x] != state2[y][x])
				return false;
	return true;
}