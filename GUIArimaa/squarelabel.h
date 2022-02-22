#ifndef SQUARELABEL_H
#define SQUARELABEL_H

#include <QString>
#include <QWidget>
#include <QLabel>

class SquareLabel : public QLabel
{
    Q_OBJECT

public:
    SquareLabel(QWidget* parent = 0);

    void setupSquare(int,int,QString,char);
    void updateSquare(QString, char); //move new piece into square
    void updateImage(); //change the image of the square
    void makeEmpty(); //make squarelabel into an empty square

    //maybe merge into a toggleActive() and toggleSelected()
    void setActive();
    void setInactive();
    void setSelected();
    void setDeselected();
    void setTrap();
    void setSelectable();
    void setUnselectable();
    void setPullable();
    void setNotPullable();

    bool isActive();
    bool isSelected();
    bool isPullable();
    bool isTrap();

    QString getPieceType() {return piece_type;}
    char getPieceColour() {return piece_colour;}
    QPixmap getImage() {return image;}

    static QString default_stylesheet;
    static QString selected_colour_style;
    static QString active_colour_style;
    static QString inactive_colour_style;
    static QString trap_colour_style;
    static QString selectable_colour_style;

signals:
    void clicked(int,int);
    void selectedSquareClicked(int,int);
    void selectableSquareClicked(int,int);

protected:
    void mousePressEvent(QMouseEvent *e);

private:

    QPixmap image; //image of piece, or none if empty square
    int row, col; //co-ordinates of the square (consistent with the internal char board representation)
    QString piece_type; //e.g. rabbit. empty for empty square
    char piece_colour; //g or s. n for none

    bool active = false;
    bool selected = false;
    bool trap = false;
    bool selectable = false;
    bool pullable = false;


};

#endif // SQUARELABEL_H
