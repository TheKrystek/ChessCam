#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QDateTime>

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
    ui->pushButton_2->hide();

    QObject::connect(ui->display, SIGNAL(newMessage(QString,int)), this, SLOT(displayInMessageBox(QString,int)));
    QObject::connect(this->chessboard, SIGNAL(moved(Point,Point)), this, SLOT(log(Point,Point)));
    QObject::connect(this->chessboard, SIGNAL(captured(Point,Point)), this, SLOT(log(Point,Point)));
    QObject::connect(ui->display, SIGNAL(whoMoves(QString)), this, SLOT(whoMoves(QString)));



    ui->display->setChessboard(this->chessboard);
    ui->display->setSize(QSize(500,500));
    ui->display->setSpeed(7);


    this->showMaximized();
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
    for (int i=0;i<replayList.length();i+=2)
        ui->display->Move(replayList[i],replayList[i+1]);
    ui->pushButton_2->hide();
}

void MainWindow::displayInMessageBox(QString msg,int p){
    if (msg == "Szach")
        p = 2;
    QString styleO, styleC;
    styleO = "<center><b>";
    styleC = "</b></center>";

    switch(p){
    case 0:
        ui->messageBox->setStyleSheet("background:#F26C4F;border:none;");
        break;
    case 1:
        ui->messageBox->setStyleSheet("background:#438CCA;border:none;");
        break;
    case 2:
        ui->messageBox->setStyleSheet("background:#ACD372;border:none;");
        break;


    }
    ui->messageBox->setText(styleO + msg + styleC);
}

void MainWindow::log(Point start, Point end){
    ui->log->appendPlainText(start.niceFormat() + " -> " + end.niceFormat());
    this->replay.append(start.toString()+" "+end.toString()+"\n");

}



void MainWindow::whoMoves(QString color){

    QString text = "<html><head/><body><p align=\"center\"><span style=\" font-size:12pt; font-weight:600;\">";
    if (color == "czarne")
        text += "Teraz ruch czarnych";
    else
        text += "Teraz ruch białych";
    text += "</span></p></body></html>";
    ui->ruch->setText(text);
}

void MainWindow::on_actionRestart_triggered()
{
    QMessageBox::StandardButton reply;
    if (!ui->display->clear)
        reply = QMessageBox::question(this, "Restart?", "Czy na pewno chcesz zrestartować grę?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes || this->ui->display->clear) {
        Chessboard * tmp = this->chessboard;
        this->chessboard = new Chessboard;
        this->ui->display->setChessboard(this->chessboard);
        delete tmp;
        this->ui->display->repaint();
        this->ui->ruch->setText("<html><head/><body><p align=\"center\"><span style=\" font-size:12pt; font-weight:600;\">Teraz ruch białych</span></p></body></html>");
        this->replay = "";
        this->replayList.clear();
        this->ui->log->clear();
        this->ui->messageBox->clear();
        QObject::connect(this->chessboard, SIGNAL(moved(Point,Point)), this, SLOT(log(Point,Point)));
        QObject::connect(this->chessboard, SIGNAL(captured(Point,Point)), this, SLOT(log(Point,Point)));
    }
}

void MainWindow::on_actionSzach_triggered()
{
    this->on_actionRestart_triggered();
    this->displayInMessageBox("Scholars mate",1);
    ui->display->Move(Point(4,1),Point(4,3));
    ui->display->Move(Point(4,6),Point(4,4));
    ui->display->Move(Point(3,0),Point(7,4));
    ui->display->Move(Point(1,7),Point(2,5));
    ui->display->Move(Point(5,0),Point(2,3));
    ui->display->Move(Point(6,7),Point(5,5));
    ui->display->Move(Point(7,4),Point(5,6));
}

void MainWindow::on_actionRoszada_kr_tka_triggered()
{
    this->on_actionRestart_triggered();
    this->displayInMessageBox("Roszada krótka",1);
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
}

void MainWindow::on_actionBicie_w_przelocie_triggered()
{
    this->on_actionRestart_triggered();
    this->displayInMessageBox("Bicie w przelocie",1);

    ui->display->Move(Point(2,1),Point(2,3));
    ui->display->Move(Point(7,6),Point(7,4));
    ui->display->Move(Point(2,3),Point(2,4));
    ui->display->Move(Point(1,6),Point(1,4));
    ui->display->Move(Point(2,4),Point(1,5));
}

void MainWindow::on_actionRoszada_triggered()
{
    this->on_actionRestart_triggered();
    this->displayInMessageBox("Roszada długa",1);
    ui->display->Move(Point(4,1),Point(4,3));
    ui->display->Move(Point(3,6),Point(3,4));
    ui->display->Move(Point(0,1),Point(0,2));
    ui->display->Move(Point(1,7),Point(2,5));
    ui->display->Move(Point(5,0),Point(2,3));
    ui->display->Move(Point(2,7),Point(3,6));
    ui->display->Move(Point(6,0),Point(7,2));
    ui->display->Move(Point(4,6),Point(4,5));
    ui->display->Move(Point(0,2),Point(0,3));
    ui->display->Move(Point(3,7),Point(4,6));
    ui->display->Move(Point(0,0),Point(0,2));
    ui->display->Move(Point(0,7),Point(3,7));
    ui->display->Move(Point(4,7),Point(2,7));
}

void MainWindow::on_actionPrzerwana_roszada_triggered()
{
    this->on_actionRestart_triggered();
    this->displayInMessageBox("Przerwana roszada",1);
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
}

void MainWindow::on_action1_triggered()
{
this->ui->display->setSpeed(1);
}

void MainWindow::on_action2_triggered()
{
this->ui->display->setSpeed(2);
}

void MainWindow::on_action3_triggered()
{
this->ui->display->setSpeed(3);
}

void MainWindow::on_action4_triggered()
{
this->ui->display->setSpeed(4);
}

void MainWindow::on_action5_triggered()
{
this->ui->display->setSpeed(5);
}

void MainWindow::on_action6_triggered()
{
this->ui->display->setSpeed(6);
}

void MainWindow::on_action7_triggered()
{
this->ui->display->setSpeed(7);
}

void MainWindow::on_action8_triggered()
{
this->ui->display->setSpeed(8);
}

void MainWindow::on_action9_triggered()
{
this->ui->display->setSpeed(9);
}

void MainWindow::on_action10_triggered()
{
this->ui->display->setSpeed(10);
}

void MainWindow::on_actionDomy_lna_triggered()
{
    this->ui->display->setSpeed(8);
}

void MainWindow::on_actionZapisz_powt_rk_gry_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Zapisz powtórkę"),"",tr("*.rep"));
    QFile *file = new QFile(fileName+".rep");
    file->open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(file);
    out << "#Zapisano "+QDateTime::currentDateTime().toString()+"\n";
    out << this->replay;
    file->close();
}

void MainWindow::on_actionWczytaj_powt_rk_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Wczytaj powtórkę"),"",tr("*.rep"));
    QFile inputFile(fileName);
    QString replayDate;
    if (inputFile.open(QIODevice::ReadOnly))
    {
       int lineNumber = 0;
       QTextStream in(&inputFile);
       while ( !in.atEnd() )
       {
          QString line = in.readLine();
          lineNumber++;
          if (lineNumber > 1){
              QStringList points = line.split(' ');
              replayList.append(Point(points[0]));
              replayList.append(Point(points[1]));
          }
          else
            replayDate = line.mid(9);
       }
       inputFile.close();
    }
    inputFile.close();
    displayInMessageBox(fileName+"<br>"+replayDate,1);
    ui->pushButton_2->show();
}
