#include <iostream>
#include "Color.h"
#include "Chessboard.h"
#include "ChessPiece.h"
#include <stdio.h>
#include "Pawn.h"

int main(){

	Chessboard szachownica;

	printf("Poczatek");
	szachownica.displayPieces();
	try {
		szachownica.deletePiece(0,1);
		//szachownica.deletePiece(1,0);
		if (szachownica.tryToMove(0,0,0,6))
			printf("\nRuch poprawny\n");
		else
			printf("\nRuch niepoprawny\n");

		szachownica.displayPiece(0,6,true);
		if (szachownica.tryToMove(0,7,0,6))
					printf("\nRuch poprawny\n");
				else
					printf("\nRuch niepoprawny\n");
	}
	catch (const char * s){
		printf("[Wyjatek] %s", s);
	}
	//szachownica.displayPieces();
	return 0;
}
