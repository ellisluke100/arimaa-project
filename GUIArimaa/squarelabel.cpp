#include "squarelabel.h"

QString SquareLabel::default_stylesheet = "border: 1px solid black;";
QString SquareLabel::active_colour_style = "background-color: #90ee90;";
QString SquareLabel::inactive_colour_style = "background-color: #FFFFFF;";
QString SquareLabel::selected_colour_style = "background-color: #0000FF;";
QString SquareLabel::trap_colour_style = "background-color: #FF0000";
QString SquareLabel::selectable_colour_style = "background-color: #90eeFF";

SquareLabel::SquareLabel(QWidget* parent)
    : QLabel(parent)
{
    setFixedSize(64,64);
}

void SquareLabel::setupSquare(int row, int col, QString type, char colour) {
    this->row = row;
    this->col = col;
    this->piece_type = type;
    this->piece_colour = colour;

    updateImage();

    this->setStyleSheet(default_stylesheet + inactive_colour_style);
}

void SquareLabel::updateSquare(QString new_type, char new_colour) {
    this->piece_type = new_type;
    this->piece_colour = new_colour;

    updateImage();
}

void SquareLabel::updateImage() {
  //  QString file_path("C:/Dev/Arimaa Icons/");
    QString file_path(":/new/ArimaaProject/");

    if (piece_type != "empty") {
        switch (piece_colour) {
        case 'g': file_path.append("g"); break;
        case 's': file_path.append("s"); break;
        }

        file_path.append(piece_type);

        this->image.load(file_path);

        image = image.scaled(this->size(), Qt::KeepAspectRatio);
        this->setPixmap(image);

    }
}

void SquareLabel::makeEmpty() {
    this->piece_type = "empty";
    this->piece_colour = 'n';
    this->image = QPixmap(); //assign to empty qpixmap image

    this->setPixmap(image);
}

void SquareLabel::mousePressEvent(QMouseEvent* e) {
    Q_UNUSED(e);

    if (this->selected) {
        emit selectedSquareClicked(this->row, this->col);
    } else if (this->selectable){
        emit selectableSquareClicked(this->row, this->col);
    } else {
        emit clicked(this->row, this->col);
    }
}

void SquareLabel::setActive() {
    if (this->piece_type == "empty") {
        this->active = true;
        this->setStyleSheet(default_stylesheet + active_colour_style);
    }
}

void SquareLabel::setInactive() {
    this->active = false;
    if (!trap) {
        this->setStyleSheet(default_stylesheet + inactive_colour_style);
    } else {
        this->setStyleSheet(default_stylesheet + trap_colour_style);
    }
}

void SquareLabel::setSelected() {
    this->selected = true;
    this->setStyleSheet(default_stylesheet + selected_colour_style);
}

void SquareLabel::setDeselected() {
    this->selected = false;
    if (!trap) {
        this->setStyleSheet(default_stylesheet + inactive_colour_style);
    } else {
        this->setStyleSheet(default_stylesheet + trap_colour_style);
    }

}

void SquareLabel::setTrap() {
    this->trap = true;
    this->setStyleSheet(default_stylesheet + trap_colour_style);
}

bool SquareLabel::isActive() {
    return (active) ? true : false;
}

bool SquareLabel::isSelected() {
    return (selected) ? true : false;
}

void SquareLabel::setSelectable() {
    this->selectable = true;
    this->setStyleSheet(default_stylesheet + selectable_colour_style);
}

void SquareLabel::setUnselectable() {
    this->selectable = false;
    if (!trap) {
        this->setStyleSheet(default_stylesheet + inactive_colour_style);
    } else {
        this->setStyleSheet(default_stylesheet + trap_colour_style);
    }
}

void SquareLabel::setPullable() {
    this->pullable = true;
}

void SquareLabel::setNotPullable() {
    this->pullable = false;
}

bool SquareLabel::isPullable() {
    return (pullable) ? true : false;
}
