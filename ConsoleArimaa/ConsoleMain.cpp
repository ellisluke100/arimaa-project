#include <iostream>
#include <string>

#include "Game.h"

//--CONSOLE MAIN--

//This file acts as the "controller" in the MVC model, other classes form the data / model
//Takes user input and provides an interface for the user to interact with

//Holds x and y coords for a square on the board
struct Square {
	int x;
	int y;
};

std::unique_ptr<Game> game(new Game()); //create game object, unique_ptr so it manages memory for us. Don't need to explicitly delete

//Convert the inputted rank into an int, to be used to index the board
int parseRank(char& rank) {

	return tolower(rank) - 97; //Implicit conversion of rank to int when subtracting
}

int getDirection() {
	char dir_input;
	int dir;

	//DIRECTION LOOP
	while (1) {
		std::cin >> dir_input;

		if (dir_input == 'x') {
			return 4; //exiting current move
		}

		if (!isdigit(dir_input)) { //if the input is not a [[[number between 0 and 3]]]
			std::cout << "Invalid direction." << std::endl;
		}
		else {
			dir = dir_input - 48; //0 is ascii 48
			return dir;
		}
	}
}

Square selectSquare(bool choosing_enemy_piece) {
	
	char rank;
	int x, y;
	std::string input;
	bool is_valid_piece;

	std::cout << "Please select a piece. (E.g. a1)" << std::endl;

	while (1) {
		std::cin >> input;

		//parse
		rank = input.at(0);
		x = 8 - atoi(&input.at(1));
		y = parseRank(rank);

		if (!game->isValidLocation(x, y)) {
			std::cout << "Square is not on the board." << std::endl;
		}
		else {
			is_valid_piece = game->isValidPiece(x, y);

			if (choosing_enemy_piece) {
				is_valid_piece = !is_valid_piece; //flip the bool, since now we're choosing the other player's piece not the current players
			}

			if (is_valid_piece && !game->isEmpty(x,y)) {
				Square square;
				square.x = x;
				square.y = y;
				return square;
			}
			else {
				std::cout << "Can't select that square!" << std::endl;
			}
		}

		
	}
}


//Allows the two players to setup their pieces in any order they want. First GOLD, then SILVER.
void setupPieces() {

	std::string piece1_input;
	std::string piece2_input;

	char swap_input;

	Square square1;
	Square square2;

	//Gold swapping
	while (1) {
		system("CLS"); //BAD
		game->displayBoard();
		std::cout << "GOLD: Is this layout okay? (y/n)" << std::endl;

		std::cin >> swap_input;

		if (swap_input == 'y') {
			break;
		}
		else if (swap_input == 'n') {
			//swap piece

			std::cout << "GOLD: Please select a piece." << std::endl;

			square1 = selectSquare(false);

			std::cout << "GOLD: Please select another piece." << std::endl;

			square2 = selectSquare(false);

			game->swapPieces(square1.x, square1.y, square2.x, square2.y);
		}
		
	}

	//game->setCurrentPlayer(Piece::Owner::SILVER);
	
	//Silver swapping	
	while (1) {
		system("CLS"); //BAD
		game->displayBoard();
		std::cout << "SILVER: Is this layout okay? (y/n)" << std::endl;

		std::cin >> swap_input;

		if (swap_input == 'y') {
			break;
		}
		else if (swap_input == 'n') {
			//swap piece

			std::cout << "SILVER: Please select a piece." << std::endl;

			square1 = selectSquare(true);

			std::cout << "SILVER: Please select another piece." << std::endl;

			square2 = selectSquare(true);

			game->swapPieces(square1.x, square1.y, square2.x, square2.y);
		}

	}

	game->setCurrentPlayer(Piece::Owner::GOLD);
}

////////////////////////////////
////						////
////		PUSHING			////
////						////
////////////////////////////////

void push() {
	
	std::string input;
	int dir;

	Square square1, square2;

	//PUSHING LOOP - while the user still wants to push
	while (1) {

		std::cout << "Enter any letter to continue push. (x to go back)" << std::endl;

		std::cin >> input;

		if (input == "x") {
			break; //returns back to the main loop
		}
		else {

			std::cout << "Enter the square of the piece you want to push with." << std::endl;

			square1 = selectSquare(false);

			std::cout << "Enter the square of the piece you want to push." << std::endl;

			square2 = selectSquare(true);

			if (!game->isAdjacent(square1.x, square1.y, square2.x, square2.y)) {
				std::cout << "Invalid selection. Pieces are not adjacent!" << std::endl;
				break;
			}
			else if (!game->isStronger(square1.x, square1.y, square2.x, square2.y)) {
				std::cout << "Cannot push a stronger piece!" << std::endl;
			}
			else {
				std::cout << "Please input a direction. Left = 0, Up = 1, Down = 2, Right = 3. x to go back" << std::endl;

				//DIRECTION LOOP
				while (1) {
					dir = getDirection();

					if (dir == 4) {
						break;
					} 
					else {
						if (game->isValidStep(square2.x, square2.y, dir, true) && !game->isFrozen(square1.x, square1.y)) {

							int dir2 = game->getDir(square1.x, square1.y, square2.x, square2.y);
							game->step(square2.x, square2.y, dir); //make the step
							game->step(square1.x, square1.y, dir2);
							
							return;

						}
						else {
							std::cout << "Cannot move in that direction, or pushing piece is frozen." << std::endl;

						}
					}
				} //END OF DIRECTION LOOP
			}

		}
	} //END OF PUSHING LOOP

}

////////////////////////////////
////						////
////		PULLING			////
////						////
////////////////////////////////

void pull() {

	std::string input;
	int dir;

	Square square1, square2;

	//PULLING LOOP - while the user still wants to pull
	while (1) {

		std::cout << "Enter any letter to continue pull. (x to go back)" << std::endl;

		std::cin >> input;

		if (input == "x") {
			break; //returns back to the main loop
		}
		else {
			std::cout << "Enter the square of the piece you want to pull with." << std::endl;
			square1 = selectSquare(false);

			std::cout << "Enter the square of the piece you want to pull." << std::endl;
			square2 = selectSquare(true);

			if (!game->isAdjacent(square1.x, square1.y, square2.x, square2.y)) {
				std::cout << "Invalid selection. Pieces are not adjacent!" << std::endl;
				break;
			}
			else if (!game->isStronger(square1.x, square1.y, square2.x, square2.y)) {
				std::cout << "Cannot pull a stronger piece!" << std::endl;
			}
			else {
				std::cout << "Please input a direction. Left = 0, Up = 1, Down = 2, Right = 3. (x to exit)" << std::endl;

				while (1) {

					dir = getDirection();

					if (dir == 4) {
						break;
					}
					else {
						if (game->isValidStep(square1.x, square1.y, dir, false)) {

							int dir2 = game->getDir(square2.x, square2.y, square1.x, square1.y);

							game->step(square1.x, square1.y, dir); //make the step
							game->step(square2.x, square2.y, dir2);

							return;

						}
						else {
							std::cout << "Cannot move in that direction, or pulling piece is frozen." << std::endl;

						}
					}
					
				}
				
			}

		}
	} //END OF PUSHING LOOP
}

//MOVING PIECE

void movePiece() {
	
	int dir;

	Square square = selectSquare(false);
	int x = square.x;
	int y = square.y;

	std::cout << "Please input a direction. Left = 0, Up = 1, Down = 2, Right = 3. (x to exit)" << std::endl;

	//DIRECTION LOOP
	while (1) {

		dir = getDirection();

		if (dir == 4) {
			break;
		}
		else {
			if (game->isValidStep(x, y, dir, false)) {
				game->step(x, y, dir); //make the step
				break;

			}
			else {
				std::cout << "Cannot move in that direciton." << std::endl;

			}
		}

		
	}
}

////////////////////////////////
////						////
////	    GAMELOOP		////
////						////
////////////////////////////////

void gameLoop() {
	bool playing = true;
	bool has_won;
	bool in_move;

	std::string input;

	int winner;

	while (playing) { //while the user wants to keep playing the game
		game->newGame();
		setupPieces();

		has_won = false;

		while (!has_won) { //while the user is in a match
			system("CLS"); //bad
			in_move = true;

			while (in_move) { //while the user has steps left
				//system("CLS"); //bad

				//Move UI
				std::cout << "Turn: " << game->getTurnNum() << std::endl;
				game->displayBoard();
				game->displayCurrentPlayer();
				std::cout << ": your move!" << std::endl;
				std::cout << game->getStepsLeft() << " steps left!" << std::endl;
				std::cout << "m to move. s to push. l to pull. y to undo move. z to undo turn. x to exit." << std::endl;

				std::cin >> input;
				
				if (input == "m") {
					movePiece();
				}
				else if (input == "s") { //push
					if (game->getStepsLeft() >= 2) {
						push();
					}
					else {
						std::cout << "Cannot push. Not enough steps left." << std::endl;
					}
				}
				else if (input == "l") { //pull
					if (game->getStepsLeft() >= 2) {
						pull();
					}
					else {
						std::cout << "Cannot pull. Not enough steps left." << std::endl;
					}
				}
				else if (input == "y") {
					game->undoStep(); //CHANGE TO UNDOMOVE
				}
				else if (input == "z") {
					game->undoMove(); //CHANGE TO UNDOTURN
				}
				else if (input == "x") {
					has_won = true; //to get out of match loop
					break;
				}

				//Check if any pieces are on trap squares
				game->doTraps();
				
				//Check if we need to switch turns
				if (game->getStepsLeft() == 0) {
					in_move = false; //end of turn. still want to check win after
				}
				
				//Check if a player has won
				winner = game->checkWin();

				if (winner == 1) { //Gold win
					game->displayBoard();
					std::cout << "GOLD has won!" << std::endl;
					in_move = false;
					has_won = true;
				}
				else if (winner == 2) { //Silver win
					game->displayBoard();
					std::cout << "SILVER has won!" << std::endl;
					in_move = false;
					has_won = true;
				}

			} //END OF MOVE LOOP

			game->changeTurns();

		} //END OF MATCH LOOP

		//check if user wants to play again
		std::cout << "Want to play again? (y/n)" << std::endl;

		std::cin >> input;

		if (input == "n") {
			playing = false;
		}

	} //END OF PLAYING LOOP
}

////////////////////////////////
////						////
////		 MAIN			////
////						////
////////////////////////////////

int main() {
	char input;

	std::cout << "ARIMAA: CONSOLE VERSION" << std::endl;
	std::cout << "Would you like to play against the computer (a) or 2 player? (b). Input (x) to exit." << std::endl;

	std::cin >> input;

	if (input == 'a') {
		std::cout << "FEATURE NOT IMPLEMENTED. SORRY." << std::endl;
	}
	else if (input == 'b') {
		gameLoop();
	}
	
	std::cout << "Thank you for playing!" << std::endl;

	char d = getchar(); //keep the window open.

	return 0;
}