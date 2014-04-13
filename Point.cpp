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

QString Point::toString(){
    QString r;
    r.append(QString("(%1;%2)").arg(posX).arg(posY));
    return r;
}

QString Point::niceFormat(){
    QString r;
    int px = 97 + posX;
    r.append(QString("%1%2").arg((char)px).arg(posY+1));
    return r;
}

Point::Point(QString str){

    QString x = str.at(1);
    QString y = str.at(3);
    this->posX = x.toInt();
    this->posY = y.toInt();

}
