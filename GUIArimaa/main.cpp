#include <QApplication>

#include "window.h"
#include "game.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    Game* game = new Game();
    Window* window = new Window(nullptr, game);
    window->resize(250,250);
    window->show();

    return app.exec();

}
