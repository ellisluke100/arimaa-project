#ifndef COMPUTERPLAYER_H
#define COMPUTERPLAYER_H

#include "board.h"

namespace EVAL_CONSTANTS {
    const int RABBIT = 15;
    const int CAT = 2;
    const int DOG = 5;
    const int HORSE = 8;
    const int CAMEL = 11;
    const int ELEPHANT = 14;

    const int WIN_VALUE = 50;
}

class ComputerPlayer
{
public:
    ComputerPlayer();


    typedef struct {
        std::pair<int,int> source;
        std::pair<int,int> dest;

        //PUSHING
        bool pushing_step = false; //signifies the step is part of a push
       // std::pair<int,int> pushing_piece_dest;
        std::vector<std::pair<int,int>> pushing_pieces; //maybe make this a pointer

        //PULLING
        std::vector<std::pair<int,int>> possible_pulled_pieces;

    } Step;

    typedef struct {
        Step step1;
        Step step2;
        Step step3;
        Step step4;
    } Move;

    Move findBestMove(Board curr_board);

private:

    int alphaBetaSearch(Board, int, int, int); //board state, alpha, beta, depth
    //INITIAL CALL DEPTH IS NOT 0! INTIIAL CALL DEPTH IS THE GOAL DEPTH!
    std::vector<Step> generateSteps(Board);
    //std::vector<Step> orderSteps();
    int evaluateState(Board&);

    bool isTerminalState(Board&);
};

#endif // COMPUTERPLAYER_H

