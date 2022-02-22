#include "game.h"

Game::Game()
{
   computer_player = new ComputerPlayer();
}

Game::~Game()
{
   delete computer_player;
}

void Game::newGame() {
    game_board.setupBoard();

    steps_left = 4;

    curr_player = Piece::Owner::GOLD;
    board_history.clear();
}

//Iterate over the 4 trap squares and remove any pieces on them and return the location of the square if a piece is removed
std::vector<std::pair<int,int>> Game::doTraps() {

    std::vector<std::pair<int,int>> trapped_squares;
    Piece* curr_piece = nullptr;

        //c3
        curr_piece = game_board.getPiece(5, 2);
        if (curr_piece->getType() != Piece::Type::EMPTY) { //if there is a piece on the trap square

            if (!hasFriendlyAdjacent(5, 2)) { //if there's no adjacent friendly piece

                //check if the piece being removed is a rabbit and deduct from the appropriate rabbit count
                if (curr_piece->getType() == Piece::Type::RABBIT) {
                    if (curr_piece->getOwner() == Piece::Owner::GOLD) {
                        game_board.decrementGoldRabbits();
                    }
                    else {
                        game_board.decrementSilverRabbits();
                    }
                }

                trapped_squares.push_back(std::pair<int,int>(5,2));
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
                        game_board.decrementGoldRabbits();
                    }
                    else {
                        game_board.decrementSilverRabbits();
                    }
                }

                 trapped_squares.push_back(std::pair<int,int>(2,2));
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
                        game_board.decrementGoldRabbits();
                    }
                    else {
                        game_board.decrementSilverRabbits();
                    }
                }

                trapped_squares.push_back(std::pair<int,int>(5,5));
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
                        game_board.decrementGoldRabbits();
                    }
                    else {
                        game_board.decrementSilverRabbits();
                    }
                }

                trapped_squares.push_back(std::pair<int,int>(2,5));
                game_board.removePiece(2, 5);
            }
        }

        return trapped_squares;
}

bool Game::hasFriendlyAdjacent(int row, int col) {
    Piece::Owner owner = game_board.getPiece(row, col)->getOwner();
        std::vector<Piece*> adjacent_pieces;
        adjacent_pieces.emplace_back(game_board.getPiece(row - 1, col));
        adjacent_pieces.emplace_back(game_board.getPiece(row, col - 1));
        adjacent_pieces.emplace_back(game_board.getPiece(row + 1, col));
        adjacent_pieces.emplace_back(game_board.getPiece(row, col + 1));

        for (Piece* piece : adjacent_pieces) {

            if (piece != nullptr) { //if square not out of bounds of the board

                if (piece->getOwner() == owner) { //if ally piece

                    return true;
                }

            }

        }

        return false;
}

//A valid source square is not empty, and the piece is not frozen
//(unless selected enemy piece - determine by comparison between curr player and owner of piece at x,y), or smth else
bool Game::isValidSourceSquare(int row, int col, bool pushable) {
   /* if (game_board.getPiece(row,col)->getType() != Piece::Type::EMPTY && !isFrozen(row,col)) {
        return true;
    } else {
        return false;
    }
*/
    //check if square is empty
    if (game_board.getPiece(row,col)->getType() == Piece::Type::EMPTY) {
        return false;
    }

    //check if square is an enemy square we are trying to push
    if (pushable) {
        return true;
    } else {
        if (game_board.getPiece(row,col)->getOwner() != curr_player) {
            return false;
        }

        if (!isFrozen(row,col)) {
            return true;
        }
    }

    return false;
}

bool Game::isStronger(Piece* a, Piece* b) {
    if (a->getStrength() > b->getStrength()) {
            return true;
        }
        else {
            return false;
    }
}

bool Game::isFrozen(int row, int col) {
    //Check surroudning enemy piece strength
        //Check surrounding ally piece strength
        //If enemy stronger, return false, else return true

        bool is_stronger_enemy = false; //true if theres any stronger adjacent enemy piece
        bool is_adjacent_ally = false; //true if theres any adjacent ally piece

        Piece* centre_piece = game_board.getPiece(row, col); //chosen piece

        //	        x-1, y
        //x,y-1		 x, y	   x,y+1
        //		    x+1,y

        std::vector<Piece*> adjacent_pieces;
        adjacent_pieces.emplace_back(game_board.getPiece(row - 1, col));
        adjacent_pieces.emplace_back(game_board.getPiece(row, col-1));
        adjacent_pieces.emplace_back(game_board.getPiece(row+1, col));
        adjacent_pieces.emplace_back(game_board.getPiece(row, col+1));

        for (Piece* piece : adjacent_pieces) {

            if (piece != nullptr) {

                //if pieceowner = curr_player
                if (piece->getOwner() == centre_piece->getOwner()) {
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

bool Game::isOwnedByCurrentPlayer(int row, int col) {
    if (game_board.getPiece(row,col)->getOwner()==curr_player) {
        return true;
    } else {
        return false;
    }
}

bool Game::hasStrongerEnemyAdjacent(int row, int col, bool push) {
    Piece* centre_piece = game_board.getPiece(row, col); //chosen piece

    //	        x-1, y
    //x,y-1		 x, y	   x,y+1
    //		    x+1,y

    Piece* piece = game_board.getPiece(row-1, col);
    if (piece != nullptr) {

        if (piece->getOwner() != centre_piece->getOwner()) {
            if (isStronger(piece, centre_piece)) {

                if (push) {
                    if (isFrozen(row-1,col)) {
                        return false;
                    }
                }
                return true;
            }
        }
    }


    piece = game_board.getPiece(row+1, col);
    if (piece != nullptr) {

        if (piece->getOwner() != centre_piece->getOwner()) {
            if (isStronger(piece, centre_piece)) {

                if (push) {
                    if (isFrozen(row+1,col)) {
                        return false;
                    }
                }
                return true;
            }
        }
    }


    piece = game_board.getPiece(row, col-1);
    if (piece != nullptr) {

        if (piece->getOwner() != centre_piece->getOwner()) {
            if (isStronger(piece, centre_piece)) {

                if (push) {
                    if (isFrozen(row,col-1)) {
                        return false;
                    }
                }
                return true;
            }
        }
    }


    piece = game_board.getPiece(row, col+1);
    if (piece != nullptr) {

        if (piece->getOwner() != centre_piece->getOwner()) {
            if (isStronger(piece, centre_piece)) {

                if (push) {
                    if (isFrozen(row,col+1)) {
                        return false;
                    }
                }
                return true;
            }
        }
    }

    return false;

}

std::vector<std::pair<int,int>> Game::getStrongerEnemyAdjacents(int dest_row, int dest_col, int row, int col) {
    std::vector<std::pair<int,int>> stronger_adjacents;

    Piece* dest_piece = game_board.getPiece(dest_row, dest_col);
    Piece* piece = game_board.getPiece(row-1, col);

    if (piece != nullptr) {
        if (piece->getOwner() != dest_piece->getOwner()) {
            if (isStronger(piece, dest_piece)) {
                std::pair<int,int> coords(row-1, col);
                stronger_adjacents.push_back(coords);
            }
        }
    }

    piece = game_board.getPiece(row+1, col);

    if (piece != nullptr) {
        if (piece->getOwner() != dest_piece->getOwner()) {
            if (isStronger(piece, dest_piece)) {
                std::pair<int,int> coords(row+1, col);
                stronger_adjacents.push_back(coords);
            }
        }
    }

    piece = game_board.getPiece(row, col-1);

    if (piece != nullptr) {
        if (piece->getOwner() != dest_piece->getOwner()) {
            if (isStronger(piece, dest_piece)) {
                std::pair<int,int> coords(row, col-1);
                stronger_adjacents.push_back(coords);
            }
        }
    }

    piece = game_board.getPiece(row, col+1);

    if (piece != nullptr) {
        if (piece->getOwner() != dest_piece->getOwner()) {
            if (isStronger(piece, dest_piece)) {
                std::pair<int,int> coords(row, col+1);
                stronger_adjacents.push_back(coords);
            }
        }
    }

    return stronger_adjacents;
}

std::vector<std::pair<int,int>> Game::getWeakerEnemyAdjacents(int dest_row, int dest_col, int row, int col) {
    std::vector<std::pair<int,int>> weaker_adjacents;

    Piece* dest_piece = game_board.getPiece(dest_row, dest_col);
    Piece* piece = game_board.getPiece(row-1, col);

    if (piece != nullptr && piece->getType()!=Piece::Type::EMPTY) {
        if (piece->getOwner() != dest_piece->getOwner()) {
            if (!isStronger(piece, dest_piece) && !isEqual(piece,dest_piece)) {
                std::pair<int,int> coords(row-1, col);
                weaker_adjacents.push_back(coords);
            }
        }
    }

    piece = game_board.getPiece(row+1, col);

    if (piece != nullptr && piece->getType()!=Piece::Type::EMPTY) {
        if (piece->getOwner() != dest_piece->getOwner()) {
            if (!isStronger(piece, dest_piece) && !isEqual(piece,dest_piece)) {
                std::pair<int,int> coords(row+1, col);
                weaker_adjacents.push_back(coords);
            }
        }
    }

    piece = game_board.getPiece(row, col-1);

    if (piece != nullptr && piece->getType()!=Piece::Type::EMPTY) {
        if (piece->getOwner() != dest_piece->getOwner()) {
            if (!isStronger(piece, dest_piece) && !isEqual(piece,dest_piece)) {
                std::pair<int,int> coords(row, col-1);
                weaker_adjacents.push_back(coords);
            }
        }
    }

    piece = game_board.getPiece(row, col+1);

    if (piece != nullptr && piece->getType()!=Piece::Type::EMPTY) {
        if (piece->getOwner() != dest_piece->getOwner()) {
            if (!isStronger(piece, dest_piece) && !isEqual(piece,dest_piece)) {
                std::pair<int,int> coords(row, col+1);
                weaker_adjacents.push_back(coords);
            }
        }
    }

    return weaker_adjacents;
}

void Game::move(int source_row, int source_col, int dest_row, int dest_col) {
    game_board.movePiece(source_row, source_col, dest_row, dest_col);

    steps_left--;
}

bool Game::isEqual(Piece* a, Piece* b) {
    if (a->getStrength() == b->getStrength()) {
        return true;
    } else {
        return false;
    }
}

void Game::changeTurns() {
    changePlayer();

    steps_left = 4;
    board_history.clear();
}

int Game::hasWon() {
    //Return 1 if gold has won, 2 if silver has won, 0 if no winner

        //Check GOLD rabbit reached SILVER back rank. If so, GOLD win
        //Check SILVER rabbit reached GOLD back rank. If so, SILVER win
        //Check SILVER has rabbits left. If not, GOLD win
        //Check GOLD has rabbits left. If not, SILVER win
        //------------------------------------
        //Check if SILVER can make any moves - if so, GOLD win

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
        if (game_board.getSilverRabbits() == 0) {
            return 1; //gold win
        }

        //does GOLD has any rabbits left
        if (game_board.getGoldRabbits() == 0) {
            return 2; //silver win
        }

        return 0; //no winner

    }

void Game::saveCurrentBoard() {
    game_board.board_steps = steps_left;
    board_history.push_back(game_board);
}

void Game::undoMove() {
    game_board = board_history.back();
    steps_left = game_board.board_steps;
    board_history.pop_back();
}

void Game::undoTurn() {
    game_board = board_history.front();
    steps_left = 4;
    board_history.pop_back();
}

bool Game::hasHistory() {
    if (board_history.size() > 0) {
        return true;
    } else {
        return false;
    }
}

void Game::swapPieces(int row1, int col1, int row2, int col2) {
    Piece piece1 = *(game_board.getPiece(row1, col1));
    Piece piece2 = *(game_board.getPiece(row2, col2));

    *(game_board.getPiece(row1, col1)) = piece2;
    *(game_board.getPiece(row2, col2)) = piece1;
}

void Game::changePlayer() {
    if (curr_player == Piece::Owner::GOLD) {
        curr_player = Piece::Owner::SILVER;
    } else {
        curr_player = Piece::Owner::GOLD;
    }
}

ComputerPlayer::Move Game::getComputerPlayerMove() {
    //struct move = iddfs(Board, 0, MAX_DEPTH) - iddfs would init call to alphaBetaSearch for each depth

    ComputerPlayer::Move full_move = computer_player->findBestMove(game_board);

    return full_move;
}
