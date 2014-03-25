#ifndef POINT_H
#define POINT_H


namespace ChessCam {
class Point
{
public:
    Point();
    Point(int,int);
    int X();
    int Y();
    void setX(int);
    void setY(int);

private:
    int posX;
    int posY;


};
}
#endif // POINT_H
