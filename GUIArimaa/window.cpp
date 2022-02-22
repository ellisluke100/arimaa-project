 #include "window.h"

#include <QTextStream>
#include <Windows.h>

Window::Window(QWidget* parent, Game* game_ptr)
    : QMainWindow(parent), game(game_ptr)
{
    board_display = new BoardView(this);

    //CONSTRUCT WINDOW
    setWindowTitle("Arimaa");

    //setWindowIcon(QIcon("path.ico"));

    v_box_layout = new QVBoxLayout();
    h_layout_top = new QHBoxLayout();
    h_layout_bottom = new QHBoxLayout();

    //SETTING UP TOP HBOX

    turn_num_label = new QLabel("Turn: ");
    gold_turn_label = new QLabel("Gold");
    silver_turn_label = new QLabel("Silver");
    finish_swap_button = new QPushButton("Finish swapping");

    h_layout_top->addWidget(turn_num_label);
    h_layout_top->addWidget(gold_turn_label);
    h_layout_top->addWidget(silver_turn_label);
    h_layout_top->addWidget(finish_swap_button);

    h_layout_top->setAlignment(Qt::AlignHCenter);

    //connect slots

    //SETTING UP BOTTOM HBOX

    undo_move_button = new QPushButton("Undo Move");
    undo_turn_button = new QPushButton("Undo Turn");
    steps_left_label = new QLabel("Steps left: ");
    quit_button = new QPushButton("Quit");

    h_layout_bottom->addWidget(undo_move_button);
    h_layout_bottom->addWidget(undo_turn_button);
    h_layout_bottom->addWidget(steps_left_label);
    h_layout_bottom->addWidget(quit_button);

    h_layout_bottom->setAlignment(Qt::AlignHCenter);

    //ADDING WIDGETS TO LAYOUT
    v_box_layout->addLayout(h_layout_top);
    v_box_layout->addWidget(board_display);
    v_box_layout->addLayout(h_layout_bottom);

    v_box_layout->setAlignment(Qt::AlignHCenter);

    //SET LAYOUT
    QWidget* content = new QWidget();
    content->setLayout(v_box_layout);
    setCentralWidget(content);

    connect(undo_move_button, SIGNAL(clicked()), this, SLOT(undoMoveClicked()));
    connect(undo_turn_button, SIGNAL(clicked()), this, SLOT(undoTurnClicked()));
    connect(quit_button, SIGNAL(clicked()), this, SLOT(quitClicked()));
    connect(finish_swap_button, SIGNAL(clicked()), this, SLOT(finishSwapClicked()));

    newGame();
}

void Window::newGame() {

    //setup a new game, creates board, inits members of game
    game->newGame();
    board_display->resetBoard();

    gold_turn_label->setStyleSheet("background-color: gold"); //display current player
    silver_turn_label->setStyleSheet("background-color: white");

    QString steps_left_string("Steps left: ");
    steps_left_label->setText(steps_left_string + QString::number(game->getStepsLeft()));

    turn_num = 1;
    QString turn_num_string("Turn: ");
    turn_num_label->setText(turn_num_string + QString::number(turn_num));

    winner = 2;
    in_push = false;
    in_pull = false;

    source_row = -1;
    source_col = -1;
    pulling_row = -1;
    pulling_col = -1;

    curr_player = 'g';

    swapping = true;
    finish_swap_button->show();

    twoPlayerOrCPU();
}

void Window::nextTurn() {
    if (curr_player == 'g') {
        curr_player = 's';
        silver_turn_label->setStyleSheet("background-color: silver");
        gold_turn_label->setStyleSheet("background-color: white");
    } else {
        curr_player = 'g';
        gold_turn_label->setStyleSheet("background-color: gold");
        silver_turn_label->setStyleSheet("background-color: white");
    }

    turn_num_label->setText("Turn: " + QString::number(turn_num++));

    board_display->setUnselectableSquares();
    board_display->emptyHistory();

    //pass control to computer player
    if (vs_computer) {
        ComputerPlayer::Move computer_move = game->getComputerPlayerMove();

        //step 1
        game->move(computer_move.step1.source.first, computer_move.step1.source.second,
                    computer_move.step1.dest.first, computer_move.step1.dest.second);

        board_display->updateSquares(computer_move.step1.source.first, computer_move.step1.source.second,
                                     computer_move.step1.dest.first, computer_move.step1.dest.second);

        std::vector<std::pair<int,int>> trapped_squares = game->doTraps();

        if (trapped_squares.size() > 0) {
            board_display->updateTraps(trapped_squares);
        }

        //step 2
        game->move(computer_move.step2.source.first, computer_move.step2.source.second,
                    computer_move.step2.dest.first, computer_move.step2.dest.second);

        board_display->updateSquares(computer_move.step2.source.first, computer_move.step2.source.second,
                                     computer_move.step2.dest.first, computer_move.step2.dest.second);

        trapped_squares = game->doTraps();

        if (trapped_squares.size() > 0) {
            board_display->updateTraps(trapped_squares);
        }

        //step 3
        game->move(computer_move.step3.source.first, computer_move.step3.source.second,
                    computer_move.step3.dest.first, computer_move.step3.dest.second);

        board_display->updateSquares(computer_move.step3.source.first, computer_move.step3.source.second,
                                     computer_move.step3.dest.first, computer_move.step3.dest.second);

        trapped_squares = game->doTraps();

        if (trapped_squares.size() > 0) {
            board_display->updateTraps(trapped_squares);
        }

        //step 4
        game->move(computer_move.step4.source.first, computer_move.step4.source.second,
                    computer_move.step4.dest.first, computer_move.step4.dest.second);

        board_display->updateSquares(computer_move.step4.source.first, computer_move.step4.source.second,
                                     computer_move.step4.dest.first, computer_move.step4.dest.second);

        trapped_squares = game->doTraps();

        if (trapped_squares.size() > 0) {
            board_display->updateTraps(trapped_squares);
        }

        curr_player = 'g';
        gold_turn_label->setStyleSheet("background-color: gold");
        silver_turn_label->setStyleSheet("background-color: white");

        turn_num_label->setText("Turn: " + QString::number(turn_num++));
        game->changeTurns();

    }
}

void Window::checkReplay(QString winner) {
    //Gold / Silver has won
    //Play again? ( YES / NO )

    QMessageBox msgBox;
    msgBox.setText(winner + " has won!");
    msgBox.setInformativeText("Would you like to play again?");

    QPushButton* yes_button = msgBox.addButton("Yes", QMessageBox::AcceptRole);
    QPushButton* no_button = msgBox.addButton("No", QMessageBox::RejectRole);

    msgBox.exec();

    if (msgBox.clickedButton() == yes_button) {

        newGame();
    } else if (msgBox.clickedButton() == no_button) {
        //thanks for playing, exit game
        exit(0); //temp
    }
}

void Window::twoPlayerOrCPU() {
    //user selects between 2 player or CPU on QMessageBox

    QMessageBox msgBox;
    msgBox.setText("Would you like to play locally against another player or against the computer?");
    QPushButton* two_player_button = msgBox.addButton("2 player", QMessageBox::AcceptRole);
    QPushButton* computer_button = msgBox.addButton("Computer", QMessageBox::RejectRole);

    msgBox.exec();

    if (msgBox.clickedButton() == two_player_button) {
        vs_computer = false;

    } else if (msgBox.clickedButton() == computer_button) {
        vs_computer = true;
    }
}

///SLOTS///

void Window::squareClicked(int row, int col) {

    if (swapping) {
        if (source_row != -1 && game->isOwnedByCurrentPlayer(row,col)) {
            //swap destination
            game->swapPieces(source_row, source_col, row, col);
            board_display->swapSquares(source_row, source_col, row, col);
            source_row = -1;
            source_col = -1;

        } else if (source_row == -1 && game->isOwnedByCurrentPlayer(row,col)) {
            //swap source
            source_row = row;
            source_col = col;
            board_display->setSelectedSquare(source_row, source_col, swapping, false);
        }



    } else if (source_row != -1 && board_display->isActiveSquare(row,col)) {
        //destination square

        //save state of board before we move
        if (!in_push) {
            board_display->saveBoard();
            game->saveCurrentBoard();
        }

        game->move(source_row, source_col, row, col);
        board_display->updateSquares(source_row, source_col, row, col);

        //if weve moved an enemy piece, we are now in a push
        if (!game->isOwnedByCurrentPlayer(row,col) && !in_pull) {
            in_push = true;

            std::vector<std::pair<int,int>> pushing_square_locations = game->getStrongerEnemyAdjacents(row,col, source_row,source_col);

            if (pushing_square_locations.size() > 1) {

                for (std::pair<int,int> coord : pushing_square_locations) {
                    board_display->setSelectableSquare(coord.first, coord.second);
                }

            } else {
                std::pair<int,int> temp = pushing_square_locations.at(0);
                board_display->setSelectedPushSquare(temp.first, temp.second, source_row, source_col);

                source_row = temp.first;
                source_col = temp.second;
            }

        } else {

            if (!in_pull && !in_push) {
                std::vector<std::pair<int,int>> pulling_square_locations = game->getWeakerEnemyAdjacents(row,col,source_row,source_col);

                if (pulling_square_locations.size() > 0) {
                    for (std::pair<int,int> square : pulling_square_locations) {
                        board_display->setSelectableSquare(square.first, square.second);
                        board_display->setPullableSquare(square.first,square.second);
                    }

                    pulling_row = source_row;
                    pulling_col = source_col;
                }
            } else {
                in_pull = false;
            }

            if (in_push) {
                in_push = false;
            }


            source_row = -1;
            source_col = -1;
        }

        //traps
        std::vector<std::pair<int,int>> trapped_squares = game->doTraps();

        if (trapped_squares.size() > 0) {
            board_display->updateTraps(trapped_squares);
        }

        //win
        if (!in_push && !in_pull) {
            int winner = game->hasWon();

            if (winner == 1) {
                //gold winner

                checkReplay("Gold");
                return;


            } else if (winner == 2) {
                //silver winner

                checkReplay("Silver");
                return;
            }
        }


        //check for next turn
        if (game->getStepsLeft() == 0) {
            game->changeTurns();
            nextTurn();

        }

        steps_left_label->setText("Steps left: " + QString::number(game->getStepsLeft()));

    }  else if (source_row == -1) {
        //source square

        if (pulling_row != -1) {
            board_display->setUnselectableSquares();
            pulling_row = -1;
            pulling_col = -1;
        }

        //if selecting a square to push
        bool pushable = false;
        if (game->getStepsLeft() >= 2) {
            if (!game->isOwnedByCurrentPlayer(row,col) && game->hasStrongerEnemyAdjacent(row,col,true)) {
                pushable = true;
            }
        }

        //if empty and not frozen (iff ally square)
        if (game->isValidSourceSquare(row, col, pushable)) {
            source_row = row;
            source_col = col;
            board_display->setSelectedSquare(row,col, swapping, pushable);


        }

    }
}

void Window::resetMove(int row, int col) {
    if (!in_push && !in_pull) {
        source_row = -1;
        source_col = -1;
        board_display->deselectSquare(row,col);
    }
}

void Window::selectableSquareClicked(int row, int col) {
    board_display->setUnselectableSquares();

    if (board_display->isPullableSquare(row,col)) { //if we've clicked on a pullable square

        board_display->setSelectedPushSquare(row,col,pulling_row,pulling_col);
        pulling_row = -1;
        pulling_col = -1;
        in_pull = true;
    } else {
        board_display->setSelectedPushSquare(row,col,source_row,source_col);
    }

    source_row = row;
    source_col = col;
}

void Window::undoMoveClicked() {

    if (game->getStepsLeft() < 4 && game->hasHistory() && !in_push && !in_pull) {
        //call game to undo the last move
        //call board view to update to the last saved board state
        game->undoMove();
        board_display->undoMove();

        steps_left_label->setText("Steps left: " + QString::number(game->getStepsLeft()));

        source_row = -1;
        source_col = -1;
    }
}

void Window::undoTurnClicked() {

    if (game->getStepsLeft() < 4 && game->hasHistory() && !in_push && !in_pull) {
        game->undoTurn();
        board_display->undoTurn();

        steps_left_label->setText("Steps left: 4");

        source_row = -1;
        source_col = -1;
    }

}

void Window::quitClicked() {
    checkReplay("No-one");
}

void Window::finishSwapClicked() {
    if (curr_player == 'g') {
        curr_player = 's';
        game->changePlayer();
        silver_turn_label->setStyleSheet("background-color: silver");
        gold_turn_label->setStyleSheet("background-color: white");
    } else {

        swapping = false;
        curr_player = 'g';
        game->changePlayer();
        silver_turn_label->setStyleSheet("background-color: white");
        gold_turn_label->setStyleSheet("background-color: gold");
        source_row = -1;
        source_col = -1;

        finish_swap_button->hide();

    }

}
