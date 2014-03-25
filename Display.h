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
    enum Shape { Line, Points, Polyline, Polygon, Rect, RoundedRect, Ellipse, Arc,
                 Chord, Pie, Path, Text, Pixmap };

    Display(QWidget *parent = 0);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
void setMessageHandler(void (*i)(QString));

signals:
void newMessage(QString,int);

public slots:
          void draw(int x);
          void setStep(int s);


    void setShape(Shape shape);
    void setPen(const QPen &pen);
    void setBrush(const QBrush &brush);
    void setAntialiased(bool antialiased);
    void setPadding(int);
    void setPaddingLeft(int);
    void setPaddingRight(int);
    void setPaddingTop(int);
    void setPaddingBottom(int);

    void setSize(QSize);
    void setGrid(bool);
    void setAxis(bool);
    void setArrows(bool);
    void setPenAxis(QPen);
    void setPenChart(QPen);
    void setPenGrid(QPen);
    void setBackground(QBrush);
    void setChartBackground(QBrush);

 void Move(Point, Point);
void setChessboard(Chessboard* chessboard);
void loadChessPieces();


protected:

    void paintEvent(QPaintEvent *event);



private:

    // Mapy z teksturami poszczegolnych pionkow
    std::map<QString,QPixmap> pieces;
    QString message;

    Chessboard* chessboard;
    int pos_x, pos_y;

    int squareSize;

    float factor;
    QRect area;

    QPainter* painter;
    int padding,padding_left,padding_right,padding_bottom,padding_top;
    //obszar wykresu zmniejszony o padding
    QPoint left_top, left_bottom, right_top, right_bottom;

    QSize size;
    int v_sep,h_sep;
    bool grid,axis,arrows,fillChart;


    QPen penAxis, penGrid, penChart;


    QPoint changeCoordinates(Point);
    int x,y,mark;
    std::list<int> kolejka;
    int step;
    Shape shape;
    QPen pen;
    QBrush brush,bgBrush;
    bool antialiased;
    QPixmap pixmap;
    //QPainter paint;
};


#endif // Display_H
