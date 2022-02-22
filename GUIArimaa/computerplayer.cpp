#include <QTextStream>

#include "computerplayer.h"

#define MAX(x,y) ((x>y)?x:y)

QTextStream stream(stdout);
static int node_count = 0;
static int prune_count = 0;

ComputerPlayer::ComputerPlayer()
{

}

Piece::Owner checkWin(Board& board_state) {
    //Return 1 if gold has won, 2 if silver has won, 0 if no winner

        Piece* curr_piece;

        //So we don't have to fetch the enum everytime
        Piece::Owner GOLD = Piece::Owner::GOLD;
        Piece::Owner SILVER = Piece::Owner::SILVER;

        for (int y = 0; y < 8; y++) { //iterate over each square at (0,y) - back row of SILVER
            curr_piece = board_state.getPiece(0, y);

            //if piece is a rabbit and owned by GOLD
            if ((curr_piece->getType() == Piece::Type::RABBIT) && (curr_piece->getOwner() == GOLD)) {
                return Piece::Owner::GOLD; //gold win
            }
        }

        //SILVER rabbit victory
        //GOLD back rank = a8 (7,0) -> h8 (7,7)

        for (int x = 0; x < 8; x++) {
            curr_piece = board_state.getPiece(7, x);

            //if piece is a rabbit and owned by SILVER
            if ((curr_piece->getType() == Piece::Type::RABBIT) && (curr_piece->getOwner() == SILVER)) {
                return Piece::Owner::SILVER; //silver win
            }
        }

        //does SILVER has any rabbits left
        if (board_state.getSilverRabbits() == 0) {
            return Piece::Owner::GOLD; //gold win
        }

        //does GOLD has any rabbits left
        if (board_state.getGoldRabbits() == 0) {
            return Piece::Owner::SILVER; //silver win
        }

        return Piece::Owner::NONE; //no winner

    }

bool hasFriendlyAdjacent(int row, int col, Board board_state) {
    Piece::Owner owner = board_state.getPiece(row, col)->getOwner();
    std::vector<Piece*> adjacent_pieces;
    adjacent_pieces.emplace_back(board_state.getPiece(row - 1, col));
    adjacent_pieces.emplace_back(board_state.getPiece(row, col - 1));
    adjacent_pieces.emplace_back(board_state.getPiece(row + 1, col));
    adjacent_pieces.emplace_back(board_state.getPiece(row, col + 1));

    for (Piece* piece : adjacent_pieces) {

        if (piece != nullptr) { //if square not out of bounds of the board

            if (piece->getOwner() == owner) { //if ally piece

                return true;
            }

        }

    }

    return false;
}

//Iterate over the 4 trap squares and remove any pieces on them and return the location of the square if a piece is removed
void doTraps(Board& board_state) {

    Piece* curr_piece = nullptr;

        //c3
        curr_piece = board_state.getPiece(5, 2);
        if (curr_piece->getType() != Piece::Type::EMPTY) { //if there is a piece on the trap square

            if (!hasFriendlyAdjacent(5, 2, board_state)) { //if there's no adjacent friendly piece

                //check if the piece being removed is a rabbit and deduct from the appropriate rabbit count
                if (curr_piece->getType() == Piece::Type::RABBIT) {
                    if (curr_piece->getOwner() == Piece::Owner::GOLD) {
                       board_state.decrementGoldRabbits();
                    }
                    else {
                       board_state.decrementSilverRabbits();
                    }
                }

                board_state.removePiece(5, 2);
            }
        }

        //c6
        curr_piece = board_state.getPiece(2, 2);
        if (curr_piece->getType() != Piece::Type::EMPTY) {

            if (!hasFriendlyAdjacent(2, 2, board_state)) {

                //check if the piece being removed is a rabbit and deduct from the appropriate rabbit count
                if (curr_piece->getType() == Piece::Type::RABBIT) {
                    if (curr_piece->getOwner() == Piece::Owner::GOLD) {
                        board_state.decrementGoldRabbits();
                    }
                    else {
                        board_state.decrementSilverRabbits();
                    }
                }


                board_state.removePiece(2, 2);
            }
        }

        //f3
        curr_piece = board_state.getPiece(5, 5);
        if (curr_piece->getType() != Piece::Type::EMPTY) {

            if (!hasFriendlyAdjacent(5, 5, board_state)) {

                //check if the piece being removed is a rabbit and deduct from the appropriate rabbit count
                if (curr_piece->getType() == Piece::Type::RABBIT) {
                    if (curr_piece->getOwner() == Piece::Owner::GOLD) {
                        board_state.decrementGoldRabbits();
                    }
                    else {
                        board_state.decrementSilverRabbits();
                    }
                }


                board_state.removePiece(5, 5);
            }
        }

        //f6
        curr_piece = board_state.getPiece(2, 5);
        if (curr_piece->getType() != Piece::Type::EMPTY) {

            if (!hasFriendlyAdjacent(2, 5, board_state)) {

                //check if the piece being removed is a rabbit and deduct from the appropriate rabbit count
                if (curr_piece->getType() == Piece::Type::RABBIT) {
                    if (curr_piece->getOwner() == Piece::Owner::GOLD) {
                       board_state.decrementGoldRabbits();
                    }
                    else {
                        board_state.decrementSilverRabbits();
                    }
                }


               board_state.removePiece(2, 5);
            }
        }
}


bool ComputerPlayer::isTerminalState(Board& board_state) {

    Piece* curr_piece;

    //So we don't have to fetch the enum everytime
    Piece::Owner GOLD = Piece::Owner::GOLD;
    Piece::Owner SILVER = Piece::Owner::SILVER;

    for (int y = 0; y < 8; y++) { //iterate over each square at (0,y) - back row of SILVER
        curr_piece = board_state.getPiece(0, y);

        //if piece is a rabbit and owned by GOLD
        if ((curr_piece->getType() == Piece::Type::RABBIT) && (curr_piece->getOwner() == GOLD)) {
            return true; //gold win
        }
    }

    //SILVER rabbit victory
    //GOLD back rank = a8 (7,0) -> h8 (7,7)

    for (int x = 0; x < 8; x++) {
        curr_piece = board_state.getPiece(7, x);

        //if piece is a rabbit and owned by SILVER
        if ((curr_piece->getType() == Piece::Type::RABBIT) && (curr_piece->getOwner() == SILVER)) {
            return true; //silver win
        }
    }

    if (board_state.getGoldRabbits() == 0 || board_state.getSilverRabbits() == 0) {
        return true;
    }

    return false;

}

int getMaterialValue(Piece::Type type) {

    if (type == Piece::Type::RABBIT) {
        //get num rabbits
        //return val according to num rabbits
        return EVAL_CONSTANTS::RABBIT;
    }

    if (type == Piece::Type::CAT) {
        return EVAL_CONSTANTS::CAT;
    }

    if (type == Piece::Type::DOG) {
        return EVAL_CONSTANTS::DOG;
    }

    if (type == Piece::Type::HORSE) {
        return EVAL_CONSTANTS::HORSE;
    }

    if (type == Piece::Type::CAMEL) {
        return EVAL_CONSTANTS::CAMEL;
    }

    if (type == Piece::Type::ELEPHANT) {
        return EVAL_CONSTANTS::ELEPHANT;
    }

    return 0; //if an empty square gets through
}

bool isStronger(Piece* a, Piece* b) {
    if (a->getStrength() > b->getStrength()) {
        return true;
    } else {
        return false;
    }
}

bool isFrozen(int row, int col, Board& board_state) {
    //Check surroudning enemy piece strength
        //Check surrounding ally piece strength
        //If enemy stronger, return false, else return true

        bool is_stronger_enemy = false; //true if theres any stronger adjacent enemy piece
        bool is_adjacent_ally = false; //true if theres any adjacent ally piece

        Piece* centre_piece = board_state.getPiece(row, col); //chosen piece

        //	        x-1, y
        //x,y-1		 x, y	   x,y+1
        //		    x+1,y

        std::vector<Piece*> adjacent_pieces;
        adjacent_pieces.emplace_back(board_state.getPiece(row - 1, col));
        adjacent_pieces.emplace_back(board_state.getPiece(row, col-1));
        adjacent_pieces.emplace_back(board_state.getPiece(row+1, col));
        adjacent_pieces.emplace_back(board_state.getPiece(row, col+1));

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

int ComputerPlayer::evaluateState(Board& board_state) {
    Piece::Owner curr_player = board_state.current_player;
    int state_value = 0;
    Piece* curr_piece = nullptr;
    Piece* testing_piece = nullptr;

    //iterating through board, evaluating each piece in turn
    for (int i = 0; i <= 7; i++) {

        for (int j = 0; j <= 7; j++) {

            curr_piece = board_state.getPiece(i,j);

            if (curr_piece != nullptr && curr_piece->getType() != Piece::Type::EMPTY) {
                //calculate material value
                state_value += getMaterialValue(curr_piece->getType());

                //evaluating mobility
                if (isFrozen(i,j,board_state)) {
                    //frozen, so deduct
                    state_value -= curr_piece->getStrength();

                } else {
                    //calculate mobility values
                    //for each square around the unfrozen piece, add its strength
                    testing_piece = board_state.getPiece(i-1,j);
                    if (testing_piece != nullptr && testing_piece->getType() == Piece::Type::EMPTY) {
                        state_value += curr_piece->getStrength();
                    }

                    testing_piece = board_state.getPiece(i+1,j);
                    if (testing_piece != nullptr && testing_piece->getType() == Piece::Type::EMPTY) {
                        state_value += curr_piece->getStrength();
                    }

                    testing_piece = board_state.getPiece(i,j-1);
                    if (testing_piece != nullptr && testing_piece->getType() == Piece::Type::EMPTY) {
                        state_value += curr_piece->getStrength();
                    }

                    testing_piece = board_state.getPiece(i,j+1);
                    if (testing_piece != nullptr && testing_piece->getType() == Piece::Type::EMPTY) {
                        state_value += curr_piece->getStrength();
                    }

                    //win check
                    if (checkWin(board_state) == board_state.current_player) {
                        //current player has won
                        state_value += EVAL_CONSTANTS::WIN_VALUE;
                    }
                }


            }

        }
    }

    //return value, either positive or negative depending on player
    if (curr_player == Piece::Owner::SILVER) {
        return state_value; //AI - maximising player

    } else { //curr player is gold
       return -state_value; //opponent - minimising player

    }
}

bool hasStrongerEnemyAdjacent(int row, int col, bool push, Board& board_state) {
    Piece* centre_piece = board_state.getPiece(row, col); //chosen piece

    //	        x-1, y
    //x,y-1		 x, y	   x,y+1
    //		    x+1,y

    Piece* piece = board_state.getPiece(row-1, col);
    if (piece != nullptr) {

        if (piece->getOwner() != centre_piece->getOwner()) {
            if (isStronger(piece, centre_piece)) {

                if (push) {
                    if (isFrozen(row-1,col,board_state)) {
                        return false;
                    }
                }
                return true;
            }
        }
    }


    piece = board_state.getPiece(row+1, col);
    if (piece != nullptr) {

        if (piece->getOwner() != centre_piece->getOwner()) {
            if (isStronger(piece, centre_piece)) {

                if (push) {
                    if (isFrozen(row+1,col,board_state)) {
                        return false;
                    }
                }
                return true;
            }
        }
    }


    piece = board_state.getPiece(row, col-1);
    if (piece != nullptr) {

        if (piece->getOwner() != centre_piece->getOwner()) {
            if (isStronger(piece, centre_piece)) {

                if (push) {
                    if (isFrozen(row,col-1, board_state)) {
                        return false;
                    }
                }
                return true;
            }
        }
    }


    piece = board_state.getPiece(row, col+1);
    if (piece != nullptr) {

        if (piece->getOwner() != centre_piece->getOwner()) {
            if (isStronger(piece, centre_piece)) {

                if (push) {
                    if (isFrozen(row,col+1,board_state)) {
                        return false;
                    }
                }
                return true;
            }
        }
    }

    return false;

}

std::vector<std::pair<int,int>> getStrongerEnemyAdjacents(int row, int col, Board& board_state) {
    std::vector<std::pair<int,int>> stronger_adjacents;

    Piece* source_piece = board_state.getPiece(row, col);
    Piece* piece = board_state.getPiece(row-1, col);

    if (piece != nullptr) {
        if (piece->getOwner() != source_piece->getOwner()) {
            if (isStronger(piece, source_piece)) {
                std::pair<int,int> coords(row-1, col);
                stronger_adjacents.push_back(coords);
            }
        }
    }

    piece = board_state.getPiece(row+1, col);

    if (piece != nullptr) {
        if (piece->getOwner() != source_piece->getOwner()) {
            if (isStronger(piece, source_piece)) {
                std::pair<int,int> coords(row+1, col);
                stronger_adjacents.push_back(coords);
            }
        }
    }

    piece = board_state.getPiece(row, col-1);

    if (piece != nullptr) {
        if (piece->getOwner() != source_piece->getOwner()) {
            if (isStronger(piece, source_piece)) {
                std::pair<int,int> coords(row, col-1);
                stronger_adjacents.push_back(coords);
            }
        }
    }

    piece = board_state.getPiece(row, col+1);

    if (piece != nullptr) {
        if (piece->getOwner() != source_piece->getOwner()) {
            if (isStronger(piece, source_piece)) {
                std::pair<int,int> coords(row, col+1);
                stronger_adjacents.push_back(coords);
            }
        }
    }

    return stronger_adjacents;
}

bool isEqual(Piece* a, Piece* b) {
    if (a->getStrength() == b->getStrength()) {
        return true;
    } else {
        return false;
    }
}

std::vector<std::pair<int,int>> getWeakerEnemyAdjacents(int dest_row, int dest_col, int row, int col, Board& board_state) {
    std::vector<std::pair<int,int>> weaker_adjacents;

    Piece* dest_piece = board_state.getPiece(dest_row, dest_col);
    Piece* piece = board_state.getPiece(row-1, col);

    if (piece != nullptr && piece->getType()!=Piece::Type::EMPTY) {
        if (piece->getOwner() != dest_piece->getOwner()) {
            if (!isStronger(piece, dest_piece) && !isEqual(piece,dest_piece)) {
                std::pair<int,int> coords(row-1, col);
                weaker_adjacents.push_back(coords);
            }
        }
    }

    piece = board_state.getPiece(row+1, col);

    if (piece != nullptr && piece->getType()!=Piece::Type::EMPTY) {
        if (piece->getOwner() != dest_piece->getOwner()) {
            if (!isStronger(piece, dest_piece) && !isEqual(piece,dest_piece)) {
                std::pair<int,int> coords(row+1, col);
                weaker_adjacents.push_back(coords);
            }
        }
    }

    piece = board_state.getPiece(row, col-1);

    if (piece != nullptr && piece->getType()!=Piece::Type::EMPTY) {
        if (piece->getOwner() != dest_piece->getOwner()) {
            if (!isStronger(piece, dest_piece) && !isEqual(piece,dest_piece)) {
                std::pair<int,int> coords(row, col-1);
                weaker_adjacents.push_back(coords);
            }
        }
    }

    piece = board_state.getPiece(row, col+1);

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

std::vector<ComputerPlayer::Step> ComputerPlayer::generateSteps(Board board_state) {
    //return all possible legal steps from the current position
    //return legal steps from each of the current player's pieces
    //return possible push steps
    //return possible pull steps

    std::vector<Step> all_steps;

    //generate pulling steps
    if (board_state.possible_pulled_pieces.size() > 0) {
        for (std::pair<int,int> step : board_state.possible_pulled_pieces) {
            Step pulled_piece_step;
            pulled_piece_step.source.first = step.first;
            pulled_piece_step.source.second = step.second;
            pulled_piece_step.dest.first = board_state.pulling_row;
            pulled_piece_step.dest.second = board_state.pulling_col;
        }

        board_state.possible_pulled_pieces.clear();
        board_state.pulling_row = 0;
        board_state.pulling_col = 0;
    }



    //generate the step to finish the push
    if (board_state.in_push) {
       Step pushing_step;

       for (std::pair<int,int>& adjacents : board_state.stronger_adjacents) {
           pushing_step.source.first = adjacents.first;
           pushing_step.source.second = adjacents.second;
           pushing_step.dest.first = board_state.source_row;
           pushing_step.dest.second = board_state.source_col;
           all_steps.push_back(pushing_step);
       }

       //end early since pushing is atomic
       return all_steps;
    }

    Piece* curr_piece;

    //naive, no pushing / pulling
    for (int i = 0; i < 8; i++) {

        for (int j = 0; j < 8; j++) {

            curr_piece = board_state.getPiece(i,j);

            //GENERATING PUSHING MOVES
            if (curr_piece->getType() != Piece::Type::EMPTY && curr_piece->getOwner() != board_state.current_player) {
                if (board_state.getStepsLeft() >= 2) { //can only push w/ 2 or more steps

                    //check if the piece can be pushed
                    if (hasStrongerEnemyAdjacent(i,j, true, board_state)) {

                        //get the stronger adjacents and init the step
                        Step push_step;
                        push_step.source.first = i;
                        push_step.source.second = j;

                        //gen up down left and right direction for the push_step
                        //UP
                        Piece* up_piece = board_state.getPiece(i+1,j);
                        if (up_piece != nullptr && up_piece->getType() == Piece::Type::EMPTY) {
                            push_step.pushing_pieces = getStrongerEnemyAdjacents(i,j,board_state);
                            push_step.pushing_step = true;
                            push_step.dest.first = i+1;
                            push_step.dest.second = j;
                            all_steps.push_back(push_step);

                        }

                        //DOWN
                        Piece* down_piece = board_state.getPiece(i-1,j);
                        if (down_piece != nullptr && down_piece->getType() == Piece::Type::EMPTY) {
                            push_step.pushing_pieces = getStrongerEnemyAdjacents(i,j,board_state);
                            push_step.pushing_step = true;
                            push_step.dest.first = i-1;
                            push_step.dest.second = j;
                            all_steps.push_back(push_step);
                        }

                        //LEFT
                        Piece* left_piece = board_state.getPiece(i,j-1);
                        if (left_piece != nullptr && left_piece->getType() == Piece::Type::EMPTY) {
                            push_step.pushing_pieces = getStrongerEnemyAdjacents(i,j,board_state);
                            push_step.pushing_step = true;
                            push_step.dest.first = i;
                            push_step.dest.second = j-1;
                            all_steps.push_back(push_step);
                        }

                        //RIGHT
                        Piece* right_piece = board_state.getPiece(i,j+1);
                        if (right_piece != nullptr && right_piece->getType() == Piece::Type::EMPTY) {
                            push_step.pushing_pieces = getStrongerEnemyAdjacents(i,j,board_state);
                            push_step.pushing_step = true;
                            push_step.dest.first = i;
                            push_step.dest.second = j+1;
                            all_steps.push_back(push_step);
                        }


                    }
                }

            }

            //only generate steps is the piece isn't frozen
            if (!isFrozen(i,j,board_state)) {

                if (curr_piece->getOwner()==board_state.current_player && curr_piece->getType() != Piece::Type::EMPTY) {
                    //try all directions
                    //UP: i+1
                    //DOWN: i-1
                    //LEFT: j-1
                    //RIGHT:  j+1

                    //UP
                    Piece* up_piece = board_state.getPiece(i+1,j);
                    if (up_piece != nullptr && up_piece->getType() == Piece::Type::EMPTY) {
                        Step up_step;
                        up_step.source.first = i;
                        up_step.source.second = j;
                        up_step.dest.first = i+1;
                        up_step.dest.second = j;
                        all_steps.push_back(up_step);
                    }

                    //DOWN
                    Piece* down_piece = board_state.getPiece(i-1,j);
                    if (down_piece != nullptr && down_piece->getType() == Piece::Type::EMPTY) {
                        Step down_step;
                        down_step.source.first = i;
                        down_step.source.second = j;
                        down_step.dest.first = i-1;
                        down_step.dest.second = j;
                        all_steps.push_back(down_step);
                    }

                    //LEFT
                    Piece* left_piece = board_state.getPiece(i,j-1);
                    if (left_piece != nullptr && left_piece->getType() == Piece::Type::EMPTY) {
                        Step left_step;
                        left_step.source.first = i;
                        left_step.source.second = j;
                        left_step.dest.first = i;
                        left_step.dest.second = j-1;
                        all_steps.push_back(left_step);
                    }

                    //RIGHT
                    Piece* right_piece = board_state.getPiece(i,j+1);
                    if (right_piece != nullptr && right_piece->getType() == Piece::Type::EMPTY) {
                        Step right_step;
                        right_step.source.first = i;
                        right_step.source.second = j;
                        right_step.dest.first = i;
                        right_step.dest.second = j+1;
                        all_steps.push_back(right_step);
                    }
                }
            }
        }
    }

    return all_steps;
}

ComputerPlayer::Move ComputerPlayer::findBestMove(Board curr_board) {
    //approach: gen  steps, for each step, make the step, run alpha beta, undo the step. do this for each of the 4 steps in the move

    std::vector<Step> current_move;
    current_move.reserve(4);
    Step best_step = Step();
    int best_val = 0;
    curr_board.current_player = Piece::Owner::SILVER; //silver is the AI colour by default
    curr_board.resetSteps();
    Board temp_board = curr_board;
    int value;
    int depth = 4;

    for (int i = 0; i < 4; i++) {

        std::vector<Step> steps = generateSteps(temp_board);
        best_step = steps.at(rand()%steps.size());
        best_val = 0;

        for (Step step : steps) {

            temp_board.movePiece(step.source.first, step.source.second, step.dest.first, step.dest.second); //make step
            temp_board.decrementSteps();
            doTraps(temp_board);

            //if we've just made a pushing move, update the board vals accordingly
            if (step.pushing_step) {
                temp_board.in_push = true;
                temp_board.stronger_adjacents = step.pushing_pieces;
                temp_board.source_row = step.source.first;
                temp_board.source_col = step.source.second;
            } else {

                    std::vector<std::pair<int,int>> possible_pulls = getWeakerEnemyAdjacents(step.dest.first, step.dest.second,
                                                                                            step.source.first, step.source.second, temp_board);
                    if (possible_pulls.size() >= 1) {
                        temp_board.possible_pulled_pieces = possible_pulls;
                        temp_board.pulling_row = step.source.first;
                        temp_board.pulling_col = step.source.second;
                    }

            }

            value = alphaBetaSearch(temp_board, 70, -70, depth); //search and get a value for the step
            temp_board = curr_board; //undo step

            if (value > best_val) { //if the step is better than what we currently have, store it
                best_val = value;
                best_step = step;
            }
        }

        //put the best step in the move, move the piece on the board
        current_move.push_back(best_step);

        curr_board.movePiece(best_step.source.first, best_step.source.second, best_step.dest.first, best_step.dest.second);
        doTraps(curr_board);
        curr_board.decrementSteps();

        //if the best step is a pushing step, we have to push the other step
        if (best_step.pushing_step) {

            //if there's at least 1 pushing piece, push the 1st one back (naive)
            if (best_step.pushing_pieces.size() >= 1) {
                Step push_step;
                push_step.source.first = best_step.pushing_pieces.at(0).first;
                push_step.source.second = best_step.pushing_pieces.at(0).second;
                push_step.dest.first = best_step.source.first;
                push_step.dest.second = best_step.source.second;
                current_move.push_back(push_step);
                i++;

            }

        }

        temp_board = curr_board;

    }

    //construct the move
    Move new_move;
    new_move.step1 = current_move.at(0);
    new_move.step2 = current_move.at(1);
    new_move.step3 = current_move.at(2);
    new_move.step4 = current_move.at(3);

    //performance
    stream << "Total Nodes: " << node_count << endl;
    stream << "Total Prunes: " << prune_count << endl;
    node_count = 0;
    prune_count = 0;

    return new_move;
}

int ComputerPlayer::alphaBetaSearch(Board board_state, int alpha, int beta, int depth) {
    node_count++;

    if (isTerminalState(board_state) || depth == 0) {
        return evaluateState(board_state);
    }

    std::vector<Step> steps = generateSteps(board_state);

    int score = -500;
    int value;

    for (Step& step : steps) {
        if (board_state.getStepsLeft() != 0) {

            Board temp_board = board_state;

            board_state.movePiece(step.source.first, step.source.second, step.dest.first, step.dest.second); //make step
            board_state.decrementSteps();

            //check for possible pulls
            if (!board_state.in_push) {
                std::vector<std::pair<int,int>> possible_pulls = getWeakerEnemyAdjacents(step.dest.first, step.dest.second,
                                                                                        step.source.first, step.source.second, board_state);

                if (possible_pulls.size() >= 1) {
                    board_state.possible_pulled_pieces = possible_pulls;
                    board_state.pulling_row = step.source.first;
                    board_state.pulling_col = step.source.second;
                }
            }


            doTraps(board_state); //remove any pieces on traps as a result of the step

            if (step.pushing_step) {
                board_state.in_push = true;
                board_state.stronger_adjacents = step.pushing_pieces;
                board_state.source_row = step.source.first;
                board_state.source_col = step.source.second;

            } else if (board_state.in_push) {
                board_state.in_push = false;
                temp_board.in_push = false;

            }

            value = alphaBetaSearch(board_state, alpha, beta, depth); /////

            board_state.movePiece(step.dest.first, step.dest.second, step.source.first, step.source.second); //undo step
            board_state = temp_board;

        } else if (board_state.getStepsLeft() == 0){ //0 steps left - next turn
            board_state.resetSteps();

            //switch players
            if (board_state.current_player == Piece::Owner::SILVER) {
                board_state.current_player = Piece::Owner::GOLD;
            } else {
                board_state.current_player = Piece::Owner::SILVER;
            }

            value = -alphaBetaSearch(board_state, -beta, -alpha, depth-1); /////

        }

        score = MAX(value,score);
        alpha = MAX(score,alpha);

        if (alpha >= beta) {
            prune_count++;
            break;
        }
    }

    return score;

}
