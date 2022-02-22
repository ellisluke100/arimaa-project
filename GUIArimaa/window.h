#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>

#include "game.h"
#include "boardview.h"

class Window : public QMainWindow
{

    Q_OBJECT

public:
    Window(QWidget* parent = 0, Game* game_ptr = nullptr);

    void newGame();
    void nextTurn();
    void checkReplay(QString winner);
    void resetGame();
    void twoPlayerOrCPU();

private slots:
    void squareClicked(int row, int col);
    void resetMove(int row, int col);
    void selectableSquareClicked(int row, int col);
    void undoMoveClicked();
    void undoTurnClicked();
    void quitClicked();
    void finishSwapClicked();


private:
    Game* game = nullptr; //model
    BoardView* board_display = nullptr; //view

    QVBoxLayout* v_box_layout;
    QHBoxLayout* h_layout_top; //above board
    QHBoxLayout* h_layout_bottom; //below board

    //Top HBox
    QLabel* turn_num_label = nullptr;
    QLabel* gold_turn_label = nullptr;
    QLabel* silver_turn_label = nullptr;
    QPushButton* finish_swap_button = nullptr;

    //Bottom HBox
    QPushButton* undo_move_button = nullptr;
    QPushButton* undo_turn_button = nullptr;
    QLabel* steps_left_label = nullptr;
    QPushButton* quit_button = nullptr;

    int turn_num = 0;

    int winner = 2; //0 for gold, 1 for silver, 2 for none

    bool swapping = false;
    bool in_push = false;
    bool in_pull = false;

    bool vs_computer;

    char curr_player;

    //Stores first square of a move
    int source_row = -1;
    int source_col = -1;
    int pulling_row = -1;
    int pulling_col = -1;
};

#endif // WINDOW_H
