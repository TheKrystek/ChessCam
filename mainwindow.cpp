#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QDateTime>
#include <QTimer>



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

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));




    ui->display->setChessboard(this->chessboard);
    ui->display->setSize(QSize(500,500));
    ui->display->setSpeed(7);

    cap.open(0);
    if(!cap.isOpened());
    observer.start(cap);

    timer->start(100);
    this->showMaximized();
    // Ustaw wartosci poczatkowe sliderow i checkboxow
    ui->h_lows->setValue(markerLower[0]);
    ui->h_ups->setValue(markerUpper[0]);
    ui->s_lows->setValue(markerLower[1]);
    ui->s_ups->setValue(markerUpper[1]);
    ui->v_lows->setValue(markerLower[2]);
    ui->v_ups->setValue(markerUpper[2]);
    ui->gc->setValue(globalContrast);
    ui->fc->setValue(figuresContrast);
    ui->wft->setValue(whiteFigureThreshold);

    ui->dr->setChecked(drawResult);
    ui->ac->setChecked(autoContrast);



    interrupted = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    ui->display->Move(ui->sx->value(),ui->sy->value(), ui->ex->value(),ui->ey->value());
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

void MainWindow::update(){

    if(!interrupted) {
        try {
            // sprawdzenie stani szachownicy
            state = observer.checkState();

            switch (state) {
            case MOVE:
            {
                puts("[wykonano ruch figury]");

                std::vector<cv::Point2i> coords;
                observer.getLastMove(coords);

                /*
                     Tutaj mo¿na wywo³ac odpowiedni¹ metodê modu³u logicznego

                     observer.getLastMove(moveCoords) zwraca przez argument
                     wektor dwu- lub czteroelementowy zawieraj¹cy kolejno
                     wspo³rzêdne x,y z przedzia³u <0;7> dla pól:
                     tego, z którego wykonano ruch i tego, na którym stanê³a figura.
                     Przypadek czteroelementowy to roszada - ruch dwóch figur
                     na wolne pole. Wtedy pierwsza para pól to król, druga wie¿a.

                     Gdy modu³ logiki gry wykryje ¿e ruch nie by³ poprawny,
                     nale¿y wywo³ac observer.undoneMove().

                    */

                if (coords.size() == 2)
                    ui->display->Move(Point(coords[0].x,coords[0].y),Point(coords[1].x,coords[1].y));
                if (coords.size() == 4)
                {
                    ui->display->Move(Point(coords[2].x,coords[2].y),Point(coords[3].x,coords[3].y));
                    ui->display->Move(Point(coords[0].x,coords[0].y),Point(coords[1].x,coords[1].y));
                }


            }
                break;

            case INCORRECT:
            {
                puts("[niepoprawny ruch]");
                displayInMessageBox("[niepoprawny ruch]",2);
                /*
                     Wykonany ruch jest nieprawid³owy (np. zniknê³o za du¿o figur
                     z szachownicy). Ten stan  te¿ siê zdarza przy problemach
                     z wykrywaniem w barzdo zmiennych warunkach œwiat³a.
                     Wtedy nale¿y w³¹czyæ kalibracjê, zobaczyæ co nie tak
                     i dostosowaæ suwaki.

                     Gracz powinien zostaæ poinformowany komunikatem o koniecznoœci
                     cofniecia przesuniêtych figur do poprzedniego u³o¿enia.
                     Modu³ wykrywania figur w nastêpnym wykrywaniu nie bêdzie
                     wykrywal ruchu tylko czeka³ a¿ wykryty stan faktycznie
                     bêdzie taki sam, jaki byl poprzednio (czyli czeka a¿ gracz
                     przywróci figury do poprawnego, poprzedniego stanu u³o¿enia).

                    */
            }
                break;

            case IMPOSSIBLE:
            {
                puts("[nie mozna wykryc stanu szachownicy]");
                displayInMessageBox("Nie mozna wykrsc szachownicy",3);
                /*
                     To siê zdarza gdy nie mo¿na znaleŸæ szachownicy na obrazie.
                     Wykrywanie szachownicy jest aktualnie aktywne
                     tylko w trybie kalibracji. Mo¿na poinformowaæ gracza
                     aby dostosowa³ suwaki.

                    */
            }
                break;

            default:
            {
                /*
                     Ostatnia mo¿liwoœæ to stan UNCHANGED. Nic siê nie zmieni³o
                     na szachownicy wiêc nie ma nic do roboty.

                    */
            }
            }
        }

        // mo¿e siê zdarzyæ, ¿e opencv, przy baardzo z³ym oœwietleniu i problemach
        // z wykryciem szachownicy, rzuca wyj¹tek
        catch (cv::Exception ex) {
            ex.formatMessage();
            puts(ex.what());

        }

    }
    ui->opencv->showImage(*(observer.getFrame()));
}



void MainWindow::on_h_ups_valueChanged(int value)
{
    markerUpper[0] = value;
    ui->h_upl->setText("H " + QString::number(value));
}

void MainWindow::on_h_lows_valueChanged(int value)
{
    markerLower[0] = value;
    ui->h_lowl->setText("H " + QString::number(value));
}




void MainWindow::on_s_lows_valueChanged(int value)
{
    markerLower[1] = value;
    ui->s_lowl->setText("S " + QString::number(value));
}



void MainWindow::on_s_ups_valueChanged(int value)
{
    markerUpper[1] = value;
    ui->s_upl->setText("S " + QString::number(value));
}

void MainWindow::on_v_lows_valueChanged(int value)
{
    markerLower[2] = value;
    ui->v_lowl->setText("V " + QString::number(value));
}


void MainWindow::on_v_ups_valueChanged(int value)
{
    markerUpper[1] = value;
    ui->v_upl->setText("V " + QString::number(value));
}

void MainWindow::on_gc_valueChanged(int value)
{
    globalContrast = value;
    ui->gcl->setText("Jasność " + QString::number(value));
}

void MainWindow::on_fc_valueChanged(int value)
{
    figuresContrast = value;
    ui->fcl->setText("Kontrast figur " + QString::number(value));
}

void MainWindow::on_wft_valueChanged(int value)
{
    whiteFigureThreshold = value;
    ui->wftl->setText("Próg białych " + QString::number(value));
}

void MainWindow::on_dr_stateChanged(int arg1)
{
    drawResult = arg1;
}

void MainWindow::on_ac_stateChanged(int arg1)
{
    autoContrast = arg1;

    ui->fc->setEnabled(!(bool)arg1);
    ui->wft->setEnabled(!(bool)arg1);
}
