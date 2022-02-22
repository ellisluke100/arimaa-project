#include "Game.h"

Game::Game() {
	board_history.reserve(4); //reserve 4 boards for the 4 steps
}

void Game::setCurrentPlayer(Piece::Owner player) {
	current_player = player;
}

void Game::newGame() {
	game_board.setupBoard();

	steps_left = 4;
	turn_num = 1;
	num_gold_rabbits = 1; //8
	num_silver_rabbits = 8; //8

	current_player = Piece::Owner::GOLD;
}

//swap pieces at (x1, y1) and (x2, y2)
void Game::swapPieces(int x1, int y1, int x2, int y2) {
	Piece piece1 = *(game_board.getPiece(x1, y1));
	Piece piece2 = *(game_board.getPiece(x2, y2));

	*(game_board.getPiece(x1, y1)) = piece2;
	*(game_board.getPiece(x2, y2)) = piece1;
}

//if current player is gold, switch to silver, vice versa
void Game::changeTurns() {
	if (current_player == Piece::Owner::GOLD) {
		current_player = Piece::Owner::SILVER;
	}
	else { //player is silver
		current_player = Piece::Owner::GOLD;
	}

	turn_num++;
	steps_left = 4;
	board_history.clear();
}

int Game::getStepsLeft() {
	return steps_left;
}

int Game::getTurnNum() {
	return turn_num;
}

int Game::getDir(int x1,int y1,int x2, int y2) {
	Piece* piece2 = game_board.getPiece(x2, y2); //so we can compare each square w/ the piece we're looking for

	//Adjacent pieces - + or -1 in any direction up down left or right
	//				[x-1,y1]
	//	[x1,y-1]	[x1,y1]		[x1,y+1]
	//				[x+1,y1]

	if (game_board.getPiece(x1 - 1, y1) == piece2) { //up
		return 1;
	}

	if (game_board.getPiece(x1, y1 - 1) == piece2) { //left
		return 0;
	}

	if (game_board.getPiece(x1, y1 + 1) == piece2) { //right
		return 3;
	}

	if (game_board.getPiece(x1 + 1, y1) == piece2) { //down
		return 2;
	}
}

//CONSOLE VER ONLY METHOD - print the current player
void Game::displayCurrentPlayer() {
	if (current_player == Piece::Owner::GOLD) {
		std::cout << "Gold";
	}
	else {
		std::cout << "Silver";
	}
}

//Check each trap to see if theres a piece to remove
void Game::doTraps() {
	//Get location of each trap
	//If there's a piece, check if theres any friendly adjacents. If not, remove piece from game (i.e. make empty).
	//If there's not a piece, nothing to do

	//Trap locations are: c3 (5,2), c6 (2,2), f3 (5,5), f6 (2,5)

	Piece* curr_piece = nullptr;

	//c3
	curr_piece = game_board.getPiece(5, 2);
	if (curr_piece->getType() != Piece::Type::EMPTY) { //if there is a piece on the trap square
		
		if (!hasFriendlyAdjacent(5, 2)) { //if there's no adjacent friendly piece
			
			//check if the piece being removed is a rabbit and deduct from the appropriate rabbit count
			if (curr_piece->getType() == Piece::Type::RABBIT) {
				if (curr_piece->getOwner() == Piece::Owner::GOLD) {
					num_gold_rabbits--;
				}
				else {
					num_silver_rabbits--;
				}
			}

			game_board.removePiece(5, 2);
		}
	}

	//c6
	curr_piece = game_board.getPiece(2, 2);
	if (curr_piece->getType() != Piece::Type::EMPTY) {
		
		if (!hasFriendlyAdjacent(2, 2)) {

			//check if the piece being removed is a rabbit and deduct from the appropriate rabbit count
			if (curr_piece->getType() == Piece::Type::RABBIT) {
				if (curr_piece->getOwner() == Piece::Owner::GOLD) {
					num_gold_rabbits--;
				}
				else {
					num_silver_rabbits--;
				}
			}

			game_board.removePiece(2, 2);
		}
	}
	
	//f3
	curr_piece = game_board.getPiece(5, 5);
	if (curr_piece->getType() != Piece::Type::EMPTY) {

		if (!hasFriendlyAdjacent(5, 5)) {

			//check if the piece being removed is a rabbit and deduct from the appropriate rabbit count
			if (curr_piece->getType() == Piece::Type::RABBIT) {
				if (curr_piece->getOwner() == Piece::Owner::GOLD) {
					num_gold_rabbits--;
				}
				else {
					num_silver_rabbits--;
				}
			}

			game_board.removePiece(5, 5);
		}
	}

	//f6
	curr_piece = game_board.getPiece(2, 5);
	if (curr_piece->getType() != Piece::Type::EMPTY) {

		if (!hasFriendlyAdjacent(2, 5)) {

			//check if the piece being removed is a rabbit and deduct from the appropriate rabbit count
			if (curr_piece->getType() == Piece::Type::RABBIT) {
				if (curr_piece->getOwner() == Piece::Owner::GOLD) {
					num_gold_rabbits--;
				}
				else {
					num_silver_rabbits--;
				}
			}

			game_board.removePiece(2, 5);
		}
	}
}

bool Game::hasFriendlyAdjacent(int x, int y) {
	//	        x-1, y		   
	//x,y-1		 x, y	   x,y+1
	//		    x+1,y

	Piece::Owner owner = game_board.getPiece(x, y)->getOwner();
	std::vector<Piece*> adjacent_pieces;
	adjacent_pieces.emplace_back(game_board.getPiece(x - 1, y));
	adjacent_pieces.emplace_back(game_board.getPiece(x, y - 1));
	adjacent_pieces.emplace_back(game_board.getPiece(x + 1, y));
	adjacent_pieces.emplace_back(game_board.getPiece(x, y + 1));

	for (Piece* piece : adjacent_pieces) {

		if (piece != nullptr) { //if square not out of bounds of the board
		
			if (piece->getOwner() == owner) { //if ally piece
			
				return true;
			}

		}

	}

	return false;
}

//Return true if the square at (x,y) is within the board
bool Game::isValidLocation(int x, int y) {
	if ((x >= 0 && x <= 7) && (y >= 0 && y <= 7)) { //board is 8x8, indexes 0 to 7
		
		return true;
	}
	
	return false;
}

//Return true if the piece at (x,y) is owned by current player
bool Game::isValidPiece(int x, int y) {
	/*if (!(game_board.getPiece(x, y)->getType() == Piece::Type::EMPTY)) { //if space is not empty
		
		if ((game_board.getPiece(x, y)->getOwner() == current_player)) {
			
			return true;
		}
	}*/

	if ((game_board.getPiece(x, y)->getOwner() == current_player)) {
		return true;
	}
	else {
		return false;
	}
}

//Return true if piece frozen by adjacent enemy piece, stronger than any adjacent ally piece
bool Game::isFrozen(int x, int y) {
	//Check surroudning enemy piece strength
	//Check surrounding ally piece strength
	//If enemy stronger, return false, else return true

	bool is_stronger_enemy = false; //true if theres any stronger adjacent enemy piece
	bool is_adjacent_ally = false; //true if theres any adjacent ally piece

	Piece* centre_piece = game_board.getPiece(x, y); //chosen piece

	//	        x-1, y		   
	//x,y-1		 x, y	   x,y+1
	//		    x+1,y

	std::vector<Piece*> adjacent_pieces;
	adjacent_pieces.emplace_back(game_board.getPiece(x - 1, y));
	adjacent_pieces.emplace_back(game_board.getPiece(x, y-1));
	adjacent_pieces.emplace_back(game_board.getPiece(x+1, y));
	adjacent_pieces.emplace_back(game_board.getPiece(x, y+1));

	for (Piece* piece : adjacent_pieces) {

		if (piece != nullptr) {
			
			if (piece->getOwner() == current_player) {
				is_adjacent_ally = true;
				break;
			}
			else if (piece->getType() == Piece::Type::EMPTY) {
			//	std::cout << "Empty square." << std::endl;
			}
			else {
				is_stronger_enemy = (is_stronger_enemy || isStronger(piece, centre_piece));

			}
		}
		else {
		//	std::cout << "Null square" << std::endl;
		}

	}

	if (is_stronger_enemy && is_adjacent_ally) {
		
		return false; //stronger enemy piece but theres an adjacent ally, so we can still move
	}
	else if (is_stronger_enemy) {
		
		return true; //frozen by stronger enemy piece
	}
	else {
	
		return false; //no stronger enemy
	}

	
}

//Return true if piece a has a higher strength than piece b
bool Game::isStronger(Piece* a, Piece* b) {
	if (a->getStrength() > b->getStrength()) {
		return true;
	}
	else {
		return false;
	}
}

//Return true if (x1,y1) stronger than (x2,y2)
bool Game::isStronger(int x1, int y1, int x2, int y2) {
	Piece* piece1 = game_board.getPiece(x1, y1);
	Piece* piece2 = game_board.getPiece(x2, y2);

	if (piece1->getStrength() > piece2->getStrength()) {
		return true;
	}
	else {
		return false;
	}
}

//Return true if piece at (x,y) is owned by owner
bool Game::isOwner(int x, int y, Piece::Owner owner) {

	if (game_board.getPiece(x, y)->getOwner() == owner ) {
		return true;
	}

	return false;
}

//Return true if the square at (x,y) is empty
bool Game::isEmpty(int x, int y) {

	if (game_board.getPiece(x, y)->getType() == Piece::Type::EMPTY) {
		return true;
	}
	else {
		return false;
	}
}

//Return true if a step can be made by the piece at (x,y) in direction dir
bool Game::isValidStep(int x, int y, int dir, bool is_push) {
	
	if (!is_push) { //dont need to check freezing if being pushed - since a stronger piece can always push a weaker piece (provided the move is valid)
		if (isFrozen(x, y)) {
			return false;
		}
	}
	
	int dest_x = 0;
	int dest_y = 0;

	//  1
	// 0 3
	//  2
	//Up = x-1, y
	//Left = x, y-1
	//Right = x, y+1
	//Down = x+1, y

	switch (dir) {
		case 0: dest_x = x; dest_y = y-1; break; //left
		case 1: dest_x = x-1; dest_y = y; break; //up
		case 2: dest_x = x+1; dest_y = y; break; //down
		case 3: dest_x = x; dest_y = y+1; break; //right
	}

	if (isValidLocation(dest_x, dest_y)) {

		if (game_board.getPiece(x, y)->getType() == Piece::Type::RABBIT) {
			if (game_board.getPiece(x, y)->getOwner() == Piece::Owner::GOLD && dir == 2) {
				return false;
			}
			else if (game_board.getPiece(x, y)->getOwner() == Piece::Owner::SILVER && dir == 1) {
				return false;
			}
			
		}

		if (game_board.getPiece(dest_x, dest_y)->getType() == Piece::Type::EMPTY) {
			return true;
		}
	}
	
	return false;
}

//Returns true if the two pieces (x1,y1) and (x2,y2) are adjacent - up down left right
bool Game::isAdjacent(int x1, int y1, int x2, int y2) {

	Piece* piece2 = game_board.getPiece(x2, y2); //so we can compare each square w/ the piece we're looking for

	//Adjacent pieces - + or -1 in any direction up down left or right
	//				[x-1,y1]
	//	[x1,y-1]	[x1,y1]		[x1,y+1]
	//				[x+1,y1]

	if (game_board.getPiece(x1 - 1, y1) == piece2) { //up
		return true;
	} 

	if (game_board.getPiece(x1, y1 - 1) == piece2) { //left
		return true;
	}

	if (game_board.getPiece(x1, y1 + 1) == piece2) { //right
		return true;
	}

	if (game_board.getPiece(x1 + 1, y1) == piece2) { //down
		return true;
	}

	return false;


}

//Moves the piece at (x,y) in the direction passed to the function. Directions are 0,1,2,3 - left, up, down, right
//  1
// 0 3
//  2
void Game::step(int x, int y, int dir) {
	//Add board to the history, before we make the step

	board_history.emplace_back(game_board);
	//4 - 4 = 0
	//4 - 3 = 1
	//4 - 2 = 2
	//4 - 1 = 3

	//Takes piece at (x,y), moves to the square in direction dir

	//Maybe put this in a function or change it
	int dest_x = 0;
	int dest_y = 0;

	//Determine destination square
	switch (dir) {
		case 0: dest_x = x; dest_y = y - 1; break; //left
		case 1: dest_x = x - 1; dest_y = y; break; //up
		case 2: dest_x = x + 1; dest_y = y; break; //down
		case 3: dest_x = x; dest_y = y + 1; break; //right
	}

	game_board.movePiece(x, y, dest_x, dest_y);
	steps_left--; //have made a step


	std::cout << "Piece moved." << std::endl;
	
}

void Game::undoStep() {
	//Only want to undo if there's actually a previous board in the array

	if (steps_left != 4) {
		game_board = board_history.back();
		board_history.pop_back();
		steps_left++; //we undoed a step
	}

	std::cout << "Cannot undo." << std::endl;

//	steps_left++; //we undoed a step
}	

void Game::undoMove() {
	
	if (steps_left != 4) {
		game_board = board_history[0]; //go back to original board at start of move
		board_history.clear();
		steps_left = 4; //undoed whole move
	}

	std::cout << "Cannot undo." << std::endl;

	//steps_left = 4; //undoed whole move
}

//Return 1 if gold has won, 2 if silver has won, 0 if no winner
int Game::checkWin() {
	//Check GOLD rabbit reached SILVER back rank. If so, GOLD win
	//Check SILVER rabbit reached GOLD back rank. If so, SILVER win
	//Check SILVER has rabbits left. If not, GOLD win
	//Check GOLD has rabbits left. If not, SILVER win
	//------------------------------------
	//Check if SILVER can make any moves - if so, GOLD win
	//Check if the only moves SILVER has is a 3rd time repeition - if so, GOLD win

	//GOLD Rabbit victory
	//loop over SILVER back rank, check if theres a GOLD rabbit anywhere, end early if there is, if reach end of loop and no rabbit, carry on checking
	//SILVER back rank = a1 -> a8 - indexes (7,0) -> (7,7)

	Piece* curr_piece;

	//So we don't have to fetch the enum everytime
	Piece::Owner GOLD = Piece::Owner::GOLD;
	Piece::Owner SILVER = Piece::Owner::SILVER;

	for (int y = 0; y < 8; y++) { //iterate over each square at (0,y) - back row of SILVER
		curr_piece = game_board.getPiece(0, y);

		//if piece is a rabbit and owned by GOLD
		if ((curr_piece->getType() == Piece::Type::RABBIT) && (curr_piece->getOwner() == GOLD)) {
			return 1; //gold win
		}
	}

	//SILVER rabbit victory
	//GOLD back rank = a8 (7,0) -> h8 (7,7)

	for (int x = 0; x < 8; x++) {
		curr_piece = game_board.getPiece(7, x);

		//if piece is a rabbit and owned by SILVER
		if ((curr_piece->getType() == Piece::Type::RABBIT) && (curr_piece->getOwner() == SILVER)) {
			return 2; //silver win
		}
	}

	//does SILVER has any rabbits left
	if (num_silver_rabbits == 0) {
		return 1; //gold win
	} 

	//does GOLD has any rabbits left
	if (num_gold_rabbits == 0) {
		return 2; //silver win
	}

	return 0; //no winner

}