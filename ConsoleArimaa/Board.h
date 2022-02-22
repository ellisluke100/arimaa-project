#pragma once

#include <iostream>

#include "Piece.h"

class Board
{
private:
	Piece internal_board[8][8]; //internal board representation, x [0..7], y [0..7]
	//Board of pieces or board of chars?

	//maybe we just don't return a pointer, do board operations in the board class... whod've thought

public:

	Board();

	void setupBoard(); //default - construct default board, no changes
	//void setupBoard(char[8][8]); //construct board passed into the function, player / computer has switched around pieces
	void displayBoard();
	Piece* getPiece(int, int); //get piece at (int,int)
	void movePiece(int, int, int, int); //move piece at the first two ints to the square at the last two ints
	void removePiece(int, int); //make piece empty at square (int,int)
};

