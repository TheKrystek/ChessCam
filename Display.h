#include <QBrush>
#include <QPen>
#include <QPixmap>
#include <QWidget>
#include <list>
#include <QPainter>
#include <map>
#include "Chessboard.h"

#ifndef Display_H
#define Display_H

using namespace ChessCam;


class Display : public QWidget
{
    Q_OBJECT

public:
    bool clear;
    Display(QWidget *parent = 0);
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

signals:
void newMessage(QString,int);
void whoMoves(QString);
public slots:


void Move(Point, Point);
void Move(int, int, int, int);
void setChessboard(Chessboard* chessboard);
void loadChessPieces();
void setSpeed(int);
void setSize(QSize);
void setAntialiased(bool);
protected:

    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent * event);

private:
int speed, margin;
int pos_x, pos_y;
    // Mapy z teksturami poszczegolnych pionkow
    std::map<QString,QPixmap> pieces;
    QString message;

    Chessboard* chessboard;

    int squareSize;

    float factor;
    QRect area;

    QPainter* painter;

    QSize size;
    int v_sep,h_sep;
    bool grid,axis,arrows,fillChart;


    QPoint changeCoordinates(Point);
    bool antialiased;
    QPixmap pixmap;
};


#endif // Display_H
