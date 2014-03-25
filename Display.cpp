#include "Display.h"
#include <QBrush>
#include <QPen>
#include <QPixmap>
#include <QWidget>
#include <QThread>
#include <QPainter>
#include <iostream>
#include <list>
#include "Sleeper.cpp"
#include <math.h>


Display::Display(QWidget *parent)
    : QWidget(parent)
{

    QPoint p = changeCoordinates(Point());
    pos_x = p.x();
    pos_y = p.y();

    squareSize = 20;
    padding = 4;
    shape = Path;
    step=5;
    mark = 2;
    antialiased = true;
    factor = 1;
    grid =1;
    axis = true;
    arrows = true;
    fillChart = true;
    v_sep = 50 ;     //pozioma strzalka
    h_sep = 20;
    srand(time(NULL));
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);

    this->loadChessPieces();

    bgBrush = QBrush(QColor(255,255,255,150));
    penAxis = QPen(Qt::black,2,Qt::SolidLine);
    penChart = QPen(Qt::black,1,Qt::SolidLine);
    penGrid = QPen(Qt::gray,1,Qt::DotLine);
}




QSize Display::minimumSizeHint() const
{
    return QSize(100, 100);
}

QSize Display::sizeHint() const
{
    return QSize(400, 200);

}

void Display::setSize(QSize size){
     this->size = size;

    // Okresl rozmiary pola szachownicy 8 + 4 marginesu
    if (this->size.width() > this->size.height())
        this->squareSize = (this->size.height() - 20) / 12;
    else
        this->squareSize = (this->size.width() - 20) / 12;
    setPadding(padding);
}

void Display::setPadding(int padding){
    this->padding = padding;
    padding_bottom = padding+4;
    padding_left = padding+4;
    padding_right = padding+12;
    padding_top = padding+12;

    area.setTopLeft(QPoint(padding_left,padding_top));
    area.setBottomRight(QPoint(size.width()-padding_right,size.height() - padding_bottom));
}

void Display::setShape(Shape shape)
{
    this->shape = shape;
    update();
}

void Display::setPen(const QPen &pen)
{
    this->pen = pen;
    update();
}

void Display::setBrush(const QBrush &brush)
{
    this->brush = brush;
    update();
}

void Display::setAntialiased(bool antialiased)
{
    this->antialiased = antialiased;
    update();
}

void Display::draw(int value){

    kolejka.push_back(value);
}


void Display::paintEvent(QPaintEvent * /* event */)
{

    QPainter painter(this);
    painter.setPen(pen);
    painter.setBrush(brush);

    if (antialiased)
        painter.setRenderHint(QPainter::Antialiasing, true);

       painter.save();

       // Wypelnij tlo szarym kolorem
       painter.fillRect(QRect(QPoint(0,0),this->size),QBrush(QColor(200,200,200)));

       // Narysuj cyferki
       for (int i = 0; i < 8 ; i++)
        painter.drawText(2*squareSize - squareSize/2, 2*squareSize + i * squareSize + squareSize/2 + 5, QString::number(8-i - 1));
       for (int i = 0; i < 8 ; i++)
        painter.drawText(2*squareSize + i * squareSize + squareSize/2 , 10*squareSize + squareSize/2 , QString::number(i  ));


       for (uint i =0; i < this->chessboard->whiteCaptured.size(); i++){
           QString piece = "white_";
           painter.drawPixmap(QPoint(2*squareSize + (i%8)*squareSize,5+(i/8)*0.8*squareSize), pieces[piece+this->chessboard->whiteCaptured[i]].scaled(squareSize,squareSize));
       }
       for (uint i =0; i < this->chessboard->blackCaptured.size(); i++){
           QString piece = "black_";
           painter.drawPixmap(QPoint(2*squareSize + (i%8)*squareSize,10*squareSize+20+(i/8)*0.8*squareSize), pieces[piece+this->chessboard->blackCaptured[i]].scaled(squareSize,squareSize));
       }

       // Przesun pedzel tak zeby zostawic margines
       painter.translate(2*squareSize,2*squareSize);


       for (int i = 0; i < 8; i++)
           for (int j = 0; j < 8; j++){
              if ((i+j)%2 == 0)
                  painter.setBrush(QBrush(QColor(255,205,160)));
              else
                  painter.setBrush(QBrush(QColor(210,140,70)));

              painter.drawRect(i*squareSize,j*squareSize,squareSize,squareSize);
           }

       // Jezeli zdefiniowano szachownice to wyswietl wszystkie nieruszajace sie pionki
       if (this->chessboard){
            for (int i=0;i<8;i++)
                 for (int j=0;j<8;j++){
                     if (!this->chessboard->isFree(Point(i,j))){
                          QString col = (chessboard->getColor(Point(i,j)) == ChessCam::white ? "white_" : "black_");
                          if (!this->chessboard->isMoving(Point(i,j)))
                            painter.drawPixmap(this->changeCoordinates(Point(i,j)), pieces[col+this->chessboard->getPieceType(Point(i,j))].scaled(squareSize,squareSize));
                          else
                            painter.drawPixmap(QPoint(pos_x,pos_y), pieces[col+this->chessboard->getPieceType(Point(i,j))].scaled(squareSize,squareSize));
                        }
                 }
       }
     painter.restore();

}

void Display::setStep(int step){
    this->step = step;
}
void Display::setPenAxis(QPen pen){
    this->penAxis = pen;
}
void Display::setPenChart(QPen pen){
    this->penChart = pen;
}
void Display::setPenGrid(QPen pen){
    this->penGrid = pen;
}
void Display::setArrows(bool val){
    this->arrows = val;
}
void Display::setGrid(bool val){
    this->grid = val;
}
void Display::setAxis(bool val){
    this->axis = val;
}
void Display::setBackground(QBrush brush){
   this->brush = brush;
}

void Display::setChartBackground(QBrush brush){
   this->bgBrush = brush;
}
void Display::setPaddingLeft(int i){
    this->padding_left = i+4;
}
void Display::setPaddingRight(int i){
    this->padding_right= i+12;
}
void Display::setPaddingTop(int i){
    this->padding_top = i+12;
}
void Display::setPaddingBottom(int i){
    this->padding_bottom = i+4;
}

QPoint Display::changeCoordinates(Point p)
{

    int x = p.X();
    int y = p.Y();
    y = (7 - y) * squareSize;
    x = x * squareSize;
    return QPoint(x,y);
}


void Display::setChessboard(Chessboard *chessboard){
    this->chessboard = chessboard;
}

void Display::Move(Point A, Point B){

    try {
        if (this->chessboard->tryToMove(A,B)){
            this->chessboard->toggleMoving(B);
            QPoint start = changeCoordinates(A);
            pos_x = start.x();
            pos_y = start.y();

            QPoint end = changeCoordinates(B);
            int step_x = (end.x() - start.x()) / 10;
            int step_y = (end.y() - start.y()) / 10;

            while(pos_x != end.x() || pos_y != end.y())
            {
                if ((abs(pos_x - end.x()) < abs(step_x)) ||
                    (abs(pos_y - end.y()) < abs(step_y)))
                    break;
                pos_x += step_x;
                pos_y += step_y;
                Sleeper::msleep(20);
                repaint();
            }
            pos_x = end.x();
            pos_y = end.y();
            repaint();

            if (this->chessboard->check())
                throw "Szach";
            this->chessboard->toggleMoving(B);
        }
    }
    catch (const char * S)
    {
       std::cout << S;
        emit newMessage(S,0);
    }

}

void Display::loadChessPieces(){
    QString list[6] = {"pawn","rook","knight","bishop","queen","king"};
    for (int i = 0; i < 6; i++){
        pieces["white_"+list[i]].load(":/pieces/white_"+list[i]+".png");
        pieces["black_"+list[i]].load(":/pieces/black_"+list[i]+".png");
    }
}



