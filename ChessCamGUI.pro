#-------------------------------------------------
#
# Project created by QtCreator 2014-03-11T18:12:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChessCamGUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Display.cpp \
    Sleeper.cpp \
    Chessboard.cc \
    ChessPiece.cc \
    Pawn.cc \
    Knight.cc \
    Queen.cc \
    Bishop.cc \
    Rook.cc  \
    King.cc \
    Point.cpp

HEADERS  += mainwindow.h \
    Display.h \
    Color.h \
    Chessboard.h \
    ChessPiece.h \
    Pawn.h \
    Knight.h \
    Queen.h \
    Bishop.h \
    Rook.h  \
    King.h \
    Point.h

FORMS    += mainwindow.ui

RESOURCES += \
    images.qrc
