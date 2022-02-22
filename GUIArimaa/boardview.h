#ifndef BOARDVIEW_H
#define BOARDVIEW_H

#include <QGridLayout>
#include <QWidget>

#include "squarelabel.h"

class BoardView : public QWidget
{

    Q_OBJECT

private:

    typedef struct {
        SquareLabel squares[8][8];
    } BoardState;


    QGridLayout* board_layout;

    SquareLabel board_squares[8][8];
    std::vector<BoardState*> board_history;

    char curr_player = 'g';

public:
    BoardView(QWidget* parent = 0);

    void setDefaultBoard(); //initialise default board of SquareLabels
    bool isActiveSquare(int row,int col);
    bool isPullableSquare(int row, int col);
    void setSelectedSquare(int row, int col, bool swapping, bool pushable);
    void deselectSquare(int row, int col);
    void setSelectableSquare(int row, int col);
    void setSelectedPushSquare(int row1, int col1, int row2, int col2);
    void setUnselectableSquares();
    void setPullableSquare(int row, int col);
    void setEmptySquare(int row, int col);
    void resetBoard();

    void swapSquares(int row1, int col1, int row2, int col2);

    void setBoard(BoardState*);
    void saveBoard();
    void undoMove();
    void undoTurn();
    void emptyHistory();

    void updateSquares(int source_row, int source_col, int dest_row, int dest_col);
    void updateTraps(std::vector<std::pair<int,int>> trapped_squares);




};


#endif // BOARDVIEW_H
