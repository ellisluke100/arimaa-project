#include "Board.h"

Board::Board() {
	//Constructor


}

void Board::setupBoard() {
	//0,0 is top left,    0,7 is top right,
	//7,0 is bottom left, 7,7 is bottom right
	//x and y are essentially reversed because the board is upside down in array form

	/*
	char default_board[8][8] = {
		// a    b    c    d    e    f   g     h
		{ 'r', 'r', 'r', 'r', 'r', 'r', 'r', 'r' }, // 8
		{ 'd', 'h', 'c', 'e', 'm', 'c', 'h', 'd' }, // 7
		{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' }, // 6
		{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' }, // 5
		{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' }, // 4
		{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' }, // 3
		{ 'D', 'H', 'C', 'M', 'E', 'C', 'H', 'D' }, // 2
		{ 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R' } }; // 1
	*/
	
	char default_board[8][8] = {
		// a    b    c    d    e    f   g     h
		{ 'r', 'r', 'r', 'r', 'r', 'r', 'r', 'r' }, // 8
		{ 'd', 'h', 'c', 'e', 'm', 'c', 'h', 'd' }, // 7
		{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' }, // 6
		{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' }, // 5
		{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' }, // 4
		{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' }, // 3
		{ 'D', 'H', 'C', 'M', 'E', 'C', 'H', 'D' }, // 2
		{ 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R' } }; // 1
	

	//rework this to be better

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			internal_board[i][j] = Piece(default_board[i][j]);
		}
	}
}

//Displays board alongside rank and file on the x and y axis
void Board::displayBoard() {
	
	std::cout << "  ---------------------------------" << std::endl;
	for (int i = 0; i < 8; i++) {
		std::cout << 8 - i;
		std::cout << " | ";

		for (int j = 0; j < 8; j++) {
			std::cout << internal_board[i][j].getChar() << " | ";
		}
		std::cout << std::endl;
		std::cout << "  ---------------------------------" << std::endl;
	}
	std::cout << "    a   b   c   d   e   f   g   h" << std::endl;
}

//Remove piece at (x,y)
void Board::removePiece(int x, int y) {
	internal_board[x][y].makeEmpty();
}

//Return pointer to the piece at (x,y)
Piece* Board::getPiece(int x, int y) {

	if ((x < 0 || x > 7) || (y < 0 || y > 7)) { //if (x,y) not within the board indexes
		return nullptr;
	}
	else {
		return &internal_board[x][y];
	}
}

void Board::movePiece(int source_x, int source_y, int dest_x, int dest_y) {
	internal_board[dest_x][dest_y] = internal_board[source_x][source_y];
	internal_board[source_x][source_y].makeEmpty();
}
