#include "Point.h"

using namespace ChessCam;

Point::Point():
posX(0), posY(0)
{}


Point::Point(int x,int y):
posX(x), posY(y)
{}


int Point::X(){
    return this->posX;
}
int Point::Y(){
    return this->posY;
}

void Point::setX(int x){
    this->posX = x;
}
void Point::setY(int y){
    this->posY = y;
}
