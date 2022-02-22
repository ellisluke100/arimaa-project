#ifndef GAME_H
#define GAME_H

#include <vector>

#include "computerplayer.h"

class Game
{
public:
    Game();
    ~Game();

    void newGame();

    void changeTurns();

    bool isValidSourceSquare(int row, int col, bool);
    bool isOwnedByCurrentPlayer(int row, int col);
    bool hasStrongerEnemyAdjacent(int row, int col, bool push);
    std::vector<std::pair<int,int>> getStrongerEnemyAdjacents(int dest_row, int dest_col, int row, int col);
    std::vector<std::pair<int,int>> getWeakerEnemyAdjacents(int dest_row, int dest_col, int row, int col);
    bool isFrozen(int row, int col);
    bool isStronger(Piece*, Piece*);
    bool isEqual(Piece*, Piece*);
    bool hasFriendlyAdjacent(int row, int col);
    bool hasHistory();
    void changePlayer();

    void swapPieces(int row1, int row2, int col1, int col2);

    void move(int source_row, int source_col, int dest_row, int dest_col);

    std::vector<std::pair<int,int>> doTraps();
    int hasWon();

    int getStepsLeft() {return steps_left;};

    void saveCurrentBoard();
    void undoMove();
    void undoTurn();

    //gets move from AI
    ComputerPlayer::Move getComputerPlayerMove();

private:
    Piece::Owner curr_player;
    int steps_left;

    Board game_board;
    std::vector<Board> board_history;

    ComputerPlayer* computer_player = nullptr;
};

#endif // GAME_H
