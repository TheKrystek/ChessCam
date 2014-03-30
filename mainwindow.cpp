#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QMessageBox>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    try{
        this->chessboard = new Chessboard;
    }
    catch (const char* s)
    {
        std::cout << s;
    }


    QObject::connect(ui->display, SIGNAL(newMessage(QString,int)), this, SLOT(displayInMessageBox(QString,int)));
    QObject::connect(ui->display, SIGNAL(newMessage(QString,int)), this, SLOT(log(QString)));
    QObject::connect(ui->display, SIGNAL(whoMoves(QString)), this, SLOT(whoMoves(QString)));



    //this->chessboard->setLoggerHandler(&logger);
    //Ustawienia wykresu
    ui->display->setChessboard(this->chessboard);
    ui->display->setSize(QSize(400,400));
    ui->display->setPadding(5);
    ui->display->setPenChart(QPen(QColor(0,80,255)));
    ui->display->setBackground(QBrush(QColor(237,237,237)));
    ui->display->setChartBackground(QBrush(QColor(243,243,243)));
    ui->display->setAxis(false);



}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
        ui->display->Move(ui->sx->value(),ui->sy->value(), ui->ex->value(),ui->ey->value());

       // ui->display->Move(0,1,0,2);
}

void MainWindow::on_pushButton_2_clicked()
{

    //Scholars mate
    /*
    ui->display->Move(Point(4,1),Point(4,3));
    ui->display->Move(Point(4,6),Point(4,4));
    ui->display->Move(Point(3,0),Point(7,4));
    ui->display->Move(Point(1,7),Point(2,5));
    ui->display->Move(Point(5,0),Point(2,3));
    ui->display->Move(Point(6,7),Point(5,5));
    ui->display->Move(Point(7,4),Point(5,6));
    */
    /*
    //krotka roszada bialych
    ui->display->Move(Point(4,1),Point(4,3));
    ui->display->Move(Point(4,6),Point(4,4));
    ui->display->Move(Point(3,0),Point(7,4));
    ui->display->Move(Point(1,7),Point(2,5));
    ui->display->Move(Point(5,0),Point(2,3));
    ui->display->Move(Point(6,7),Point(5,5));
    ui->display->Move(Point(6,0),Point(7,2));
    ui->display->Move(Point(5,7),Point(2,4));
    ui->display->Move(Point(7,0),Point(5,0));
    ui->display->Move(Point(4,0),Point(6,0));
    */
    /*
    //zamiast krotkiej roszady ruch czarnych
    ui->display->Move(Point(4,1),Point(4,3));
    ui->display->Move(Point(4,6),Point(4,4));
    ui->display->Move(Point(3,0),Point(7,4));
    ui->display->Move(Point(1,7),Point(2,5));
    ui->display->Move(Point(5,0),Point(2,3));
    ui->display->Move(Point(6,7),Point(5,5));
    ui->display->Move(Point(6,0),Point(7,2));
    ui->display->Move(Point(5,7),Point(2,4));
    ui->display->Move(Point(7,0),Point(5,0));
    ui->display->Move(Point(0,6),Point(0,4));
*/

    // Przerwana roszada bialych i roszada czarnych
    ui->display->Move(Point(4,1),Point(4,3));
    ui->display->Move(Point(4,6),Point(4,4));
    ui->display->Move(Point(3,0),Point(7,4));
    ui->display->Move(Point(1,7),Point(2,5));
    ui->display->Move(Point(5,0),Point(2,3));
    ui->display->Move(Point(6,7),Point(5,5));
    ui->display->Move(Point(6,0),Point(7,2));
    ui->display->Move(Point(5,7),Point(2,4));
    ui->display->Move(Point(7,0),Point(5,0));
    ui->display->Move(Point(0,6),Point(0,4));
    ui->display->Move(Point(0,1),Point(0,3));
    ui->display->Move(Point(7,7),Point(5,7));
    ui->display->Move(Point(4,7),Point(6,7));



    //ui->display->Move(Point(1,7),Point(2,5));
    //ui->display->Move(Point(5,0),Point(2,3));
    ui->display->Move(Point(6,7),Point(5,5)); // tu sie wyjebuje



}

void MainWindow::displayInMessageBox(QString msg,int p){
    QString styleO, styleC;
    styleO = "<center><b>";
    styleC = "</b></center>";

    switch(p){
    case 0:
        ui->messageBox->setStyleSheet("background:red;border:none;");
        break;
    case 1:
        ui->messageBox->setStyleSheet("background:yellow;border:none;");
        break;
    case 2:
        ui->messageBox->setStyleSheet("background:green;border:none;");
        break;


    }
    ui->messageBox->setText(styleO + msg + styleC);
}

void MainWindow::log(QString msg){

    ui->log->appendPlainText(msg);
}

void MainWindow::logger(const char * msg){

    ui->log->appendPlainText(msg);
}

void MainWindow::whoMoves(QString color){

    ui->ruch->setText(color);
}
