#include "boardview.h"

BoardView::BoardView(QWidget* parent) : QWidget(parent)
{
    setFixedSize(64*8,64*8); //board size
    board_layout = new QGridLayout(this);
    board_layout->setSpacing(0);
    board_layout->setVerticalSpacing(0);

    this->setLayout(board_layout);

    setDefaultBoard();

    //better way to do all this
    //connect all squares signal to a slot.
    for (int i = 0; i <= 7; i++) {
        for (int j = 0; j <=7; j++) {
            connect(&board_squares[i][j], SIGNAL(clicked(int,int)), parentWidget(), SLOT(squareClicked(int,int)));
            connect(&board_squares[i][j], SIGNAL(selectedSquareClicked(int,int)), parentWidget(), SLOT(resetMove(int,int)));
            connect(&board_squares[i][j], SIGNAL(selectableSquareClicked(int,int)), parentWidget(), SLOT(selectableSquareClicked(int,int)));
        }
    }

}

//initialise default board of SquareLabels
void BoardView::setDefaultBoard() {
    int i,j;

    //Silver rabbits (0,0) -> (0,7) //loop
    for (i = 0; i <= 7; i++) {
        board_squares[0][i].setupSquare(0,i,"rabbit",'s');
        board_layout->addWidget(&board_squares[0][i], 0,i);

    }

    //Silver pieces (1,0) -> (1,7)

    board_squares[1][0].setupSquare(1,0,"dog",'s');
    board_layout->addWidget(&board_squares[1][0], 1,0);

    board_squares[1][1].setupSquare(1,1,"horse",'s');
    board_layout->addWidget(&board_squares[1][1], 1,1);

    board_squares[1][2].setupSquare(1,2,"cat",'s');
    board_layout->addWidget(&board_squares[1][2], 1,2);

    board_squares[1][3].setupSquare(1,3,"elephant",'s');
    board_layout->addWidget(&board_squares[1][3], 1,3);

    board_squares[1][4].setupSquare(1,4,"camel",'s');
    board_layout->addWidget(&board_squares[1][4], 1,4);

    board_squares[1][5].setupSquare(1,5,"cat",'s');
    board_layout->addWidget(&board_squares[1][5], 1,5);

    board_squares[1][6].setupSquare(1,6,"horse",'s');
    board_layout->addWidget(&board_squares[1][6], 1,6);

    board_squares[1][7].setupSquare(1,7,"dog",'s');
    board_layout->addWidget(&board_squares[1][7], 1,7);

    //Empty squares (2,0) -> (2,7), (3,0) -> (3,7), (4,0) -> (4,7), (5,0) -> (5,7)

    for (i = 2; i <= 5; i++) {
        for (j = 0; j <= 7; j++) {
            board_squares[i][j].setupSquare(i,j,"empty",'n');
            board_layout->addWidget(&board_squares[i][j],i,j);
        }
    }

    //Gold pieces (6,0) -> (6,7)

    board_squares[6][0].setupSquare(6,0,"dog",'g');
    board_layout->addWidget(&board_squares[6][0], 6,0);

    board_squares[6][1].setupSquare(6,1,"horse",'g');
    board_layout->addWidget(&board_squares[6][1], 6,1);

    board_squares[6][2].setupSquare(6,2,"cat",'g');
    board_layout->addWidget(&board_squares[6][2], 6,2);

    board_squares[6][3].setupSquare(6,3,"elephant",'g');
    board_layout->addWidget(&board_squares[6][3], 6,3);

    board_squares[6][4].setupSquare(6,4,"camel",'g');
    board_layout->addWidget(&board_squares[6][4], 6,4);

    board_squares[6][5].setupSquare(6,5,"cat",'g');
    board_layout->addWidget(&board_squares[6][5], 6,5);

    board_squares[6][6].setupSquare(6,6,"horse",'g');
    board_layout->addWidget(&board_squares[6][6], 6,6);

    board_squares[6][7].setupSquare(6,7,"dog",'g');
    board_layout->addWidget(&board_squares[6][7], 6,7);

    //Gold rabbits (7,0) -> (7,7)
    for (i = 0; i <= 7; i++) {

        board_squares[7][i].setupSquare(7,i,"rabbit",'g');
        board_layout->addWidget(&board_squares[7][i], 7,i);

    }



    //traps
    board_squares[2][2].setTrap();
    board_squares[2][5].setTrap();
    board_squares[5][5].setTrap();
    board_squares[5][2].setTrap();
}

void BoardView::resetBoard() {
    for (int i = 0; i <= 7; i++) {
        for (int j = 0; j <= 7; j++) {
            board_squares[i][j].clear();
        }
    }
    board_history.clear();
    setDefaultBoard();
}

bool BoardView::isActiveSquare(int row, int col) {
    return (board_squares[row][col].isActive()) ? true : false;
}

void BoardView::setSelectedSquare(int row, int col, bool swapping, bool pushable) {
    //set the source square as selected, adjacent as active
    //active represents "we can move there"
    //cant active a square with a piece on, or a square outside bounds

    board_squares[row][col].setSelected();

    //up = row-1
    //down = row+1
    //left = col-1
    //right = col+1

    if (!swapping) {

        if (pushable | !(board_squares[row][col].getPieceType() == "rabbit" && board_squares[row][col].getPieceColour()=='g') ) {
            if (row+1 <= 7) {
                board_squares[row+1][col].setActive();
            }
        }


        if (pushable | !(board_squares[row][col].getPieceType() == "rabbit" && board_squares[row][col].getPieceColour()=='s')) {
            if (row-1 >= 0) {
                board_squares[row-1][col].setActive();
            }
        }



        if (col-1 >= 0) {
            board_squares[row][col-1].setActive();
         }




        if (col+1 <= 7) {
            board_squares[row][col+1].setActive();
        }

    }


}

void BoardView::deselectSquare(int row, int col) {
    board_squares[row][col].setDeselected();

    if (row+1 <= 7) {
        board_squares[row+1][col].setInactive();
    }

    if (row-1 >= 0) {
        board_squares[row-1][col].setInactive();
    }

    if (col-1 >= 0) {
        board_squares[row][col-1].setInactive();
    }

    if (col+1 <= 7) {
        board_squares[row][col+1].setInactive();
    }
}

void BoardView::setSelectableSquare(int row, int col) {
    board_squares[row][col].setSelectable();
}

void BoardView::setSelectedPushSquare(int row1, int col1, int row2, int col2) {
    board_squares[row1][col1].setSelected(); //pushing square

    board_squares[row2][col2].setActive(); //square the pushing square has to move into
}

void BoardView::setPullableSquare(int row, int col) {
    board_squares[row][col].setPullable();
}

bool BoardView::isPullableSquare(int row, int col) {

    if (board_squares[row][col].isPullable()) {
        return true;
    } else {
        return false;
    }
}

void BoardView::setUnselectableSquares() {
    for (int i = 0; i <= 7; i++) {
        for (int j = 0; j <= 7; j++) {
            board_squares[i][j].setUnselectable();
        }
    }
}

void BoardView::setEmptySquare(int row, int col) {
    board_squares[row][col].makeEmpty();
}

void BoardView::updateSquares(int source_row, int source_col, int dest_row, int dest_col) {
    //piece at [s_row][s_col] has moved to [d_row][d_col]
    //Set adjacent squares to source square inactive, deselect source square
    //Set source to dest, vice versa

    deselectSquare(source_row, source_col);

    SquareLabel* source = &board_squares[source_row][source_col];
    SquareLabel* dest = &board_squares[dest_row][dest_col];

    QString source_type = source->getPieceType();
    char source_colour = source->getPieceColour();

    source->makeEmpty();
    dest->updateSquare(source_type, source_colour);

}

void BoardView::updateTraps(std::vector<std::pair<int,int>> trapped_squares) {

    for (std::pair<int,int>& square : trapped_squares) {
        setEmptySquare(square.first, square.second);
    }


}

void BoardView::setBoard(BoardState* s) {
    QString type;
    char colour;

    for (int i = 0; i <= 7; i++) {
        for (int j = 0; j <= 7; j++) {
            type = s->squares[i][j].getPieceType();
            colour = s->squares[i][j].getPieceColour();

            if (type == "empty") {
                setEmptySquare(i,j);
            } else {
                board_squares[i][j].setupSquare(i,j,type,colour);
            }

            board_squares[i][j].setUnselectable();
            board_squares[i][j].setDeselected();
            board_squares[i][j].setInactive();
        }
    }

    board_squares[2][2].setTrap();
    board_squares[2][5].setTrap();
    board_squares[5][5].setTrap();
    board_squares[5][2].setTrap();
}

void BoardView::emptyHistory() {
    board_history.clear();
}

void BoardView::saveBoard() {
    //save current board state to board history
    BoardState* s = new BoardState;
    QString type;
    char colour;

    for (int i = 0; i <= 7; i++) {
        for (int j = 0; j <= 7; j++) {
            type = board_squares[i][j].getPieceType();
            colour = board_squares[i][j].getPieceColour();

            s->squares[i][j].setupSquare(i,j,type,colour);
        }
    }

    board_history.push_back(s);

}

void BoardView::undoMove() {
    //Retrieve the last board state from the vector (via ptr)
    //Update the current board
    //Delete the memory of the last board state

    BoardState* s = board_history.back();
    setBoard(s);
    board_history.pop_back();

    delete(s);

}

void BoardView::undoTurn() {
    BoardState *s = board_history.front();
    setBoard(s);
    board_history.clear();

    delete(s);
}

void BoardView::swapSquares(int row1, int col1, int row2, int col2) {
    SquareLabel* square1 = &board_squares[row1][col1];
    SquareLabel* square2 = &board_squares[row2][col2];

    QString type = square1->getPieceType();
    char colour = square1->getPieceColour();

    QString type2 = square2->getPieceType();
    char colour2 = square2->getPieceColour();

    board_squares[row2][col2].setupSquare(row2,col2,type,colour);
    board_squares[row1][col1].setDeselected();
    board_squares[row1][col1].setupSquare(row1,col1,type2,colour2);
}
