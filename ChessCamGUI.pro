#-------------------------------------------------
#
# Project created by QtCreator 2014-03-11T18:12:15
#
#-------------------------------------------------

QT       += core gui opengl

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
    Point.cpp \
    camera/StateDetector.cpp \
    camera/MoveRecognizer.cpp \
    camera/ChessboardState.cpp \
    camera/ChessboardDetector.cpp \
    camera/FiguresFinder.cpp \
    camera/ChessboardOvserver.cpp \
    camera/WidgetOpenCV.cpp

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
    Point.h \
    camera/MoveRecognizer.h \
    camera/StateDetector.h \
    camera/ChessboardState.h \
    camera/ChessboardDetection.h \
    camera/FiguresFinder.h \
    camera/ChessboardDetector.h \
    camera/ChessboardObserver.h \
    camera/WidgetOpenCV.h

INCLUDEPATH += /usr/include/openc2
PKGCONFIG += opencv phonon
LIBS += -LC:/usr/local/lib/ \
-lopencv_core \
-lopencv_highgui \
-lopencv_imgproc \
-lopencv_features2d \
-lopencv_calib3d \
-lopencv_video

FORMS    += mainwindow.ui

RESOURCES += \
    images.qrc
