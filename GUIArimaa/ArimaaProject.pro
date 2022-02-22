TEMPLATE = app
TARGET = ArimaaProject

QT += core gui widgets

SOURCES += \
    board.cpp \
    boardview.cpp \
    computerplayer.cpp \
    game.cpp \
    main.cpp \
    piece.cpp \
    squarelabel.cpp \
    window.cpp

HEADERS += \
    board.h \
    boardview.h \
    computerplayer.h \
    game.h \
    piece.h \
    squarelabel.h \
    window.h

RESOURCES += \
    PieceImages.qrc
