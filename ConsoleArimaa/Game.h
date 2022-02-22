#pragma once

#include <vector>

#include "Board.h"

class Game
{
public:

	Game();

	void newGame(); //sets up board and inits variables
	void swapPieces(int, int, int, int); //swaps the pieces at (int,int) and (int,int)

	void displayBoard() { game_board.displayBoard(); }

	void step(int, int, int); //encapsualtes a single step i.e. moving a piece on square in a direction

	void doTraps(); //check each of the trap squares for piece removal
	int checkWin(); //returns 1 if gold has won, 2 if silver has won, 0 if no winner

	bool isFrozen(int, int); //return true if the piece is frozen i.e there's a stronger enemy piece adjacent
	bool isValidStep(int, int, int, bool); //return true if the piece at (int,int) can move in the direction passed in. Bool means that the piece is being pushed / pulled,
	//dont need to check freezing
	bool isValidLocation(int, int); //returns true if the location at (int,int) is a valid board location	
	bool isValidPiece(int, int); //returns true if the piece at (int,int) is owned by current player
	bool isEmpty(int, int); //returns true if the square at (int,int) is empty

	bool isStronger(Piece*, Piece*); //return true if first piece stronger thn second piece
	bool isStronger(int, int, int, int); //return true if the first piece (int,int) is stronger than the second piece (int,int).

	bool isOwner(int, int, Piece::Owner); //return true if the piece at (int,int) is owned by Piece::Owner
	bool isAdjacent(int, int, int, int); //returns true if the two pieces at (int,int) and (int,int) are adjacent
	bool hasFriendlyAdjacent(int, int); //return true if theres is a friendly adjacent piece to the square at (int,int)

	void undoStep(); //go back to previous board before step was made
	void undoMove(); //go back to board at start of move

	void changeTurns(); //switches current player
	int getStepsLeft(); //return the number of remaining steps in the turn
	int getTurnNum(); //return turn number

	void displayCurrentPlayer(); //CONSOLE VER METHOD ONLY
	void setCurrentPlayer(Piece::Owner); //temp
	int getDir(int, int, int, int); //return direction that the 2nd piece is in compared to 1st piece
	


private:
	Board game_board;

	//std::array<Board, 4> board_history; //stores previous boards in the current move so player can undo their move / step
	std::vector<Board> board_history;

	Piece::Owner current_player = Piece::Owner::GOLD; //player whose turn it is - gold starts

	int num_gold_rabbits = NULL; //the number of gold rabbits
	int num_silver_rabbits = NULL; //the number of silver rabbits
	int turn_num = NULL; //the turn number
	int steps_left = NULL; //how many steps are left in the current move

};

