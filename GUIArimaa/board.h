#ifndef BOARD_H
#define BOARD_H

#include "piece.h"

class Board
{
public:

    Board();

    void setupBoard(); //default - construct default board, no changes
    void displayBoard();
    Piece* getPiece(int, int); //get piece at (int,int)
    void movePiece(int, int, int, int); //move piece at the first two ints to the square at the last two ints
    void removePiece(int, int); //make piece empty at square (int,int)
    int getStepsLeft() { return board_steps;};
    void resetSteps() { board_steps = 4;};
    int getGoldRabbits() { return num_gold_rabbits;};
    int getSilverRabbits() { return num_silver_rabbits;};

    void decrementGoldRabbits() { num_gold_rabbits--;};
    void decrementSilverRabbits() { num_silver_rabbits--;};
    void decrementSteps();

    //For the computer player
    int board_steps = 4;
    int num_gold_rabbits;
    int num_silver_rabbits;
    Piece::Owner current_player;

    bool in_push = false;
    //row + col of pushed piece
    int source_row;
    int source_col;
    std::vector<std::pair<int,int>> stronger_adjacents; //row + col of pushing piece(s)

    //pulling
    int pulling_row;
    int pulling_col;
    std::vector<std::pair<int,int>> possible_pulled_pieces;

private:

    Piece internal_board[8][8]; //internal board representation, x [0..7], y [0..7]
    //Board of pieces or board of chars?

    //maybe we just don't return a pointer, do board operations in the board class... whod've thought


};

#endif // BOARD_H
