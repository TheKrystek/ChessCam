#ifndef POINT_H
#define POINT_H
#include <QString>

namespace ChessCam {
class Point
{
public:
    Point();
    Point(int,int);
    Point(QString);
    int X();
    int Y();
    void setX(int);
    void setY(int);
    QString toString();
    QString niceFormat();

private:
    int posX;
    int posY;


};
}
#endif // POINT_H
