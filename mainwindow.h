#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "Chessboard.h"
#include <QMainWindow>
#include "Point.h"
#include "camera/ChessboardObserver.h"


using namespace ChessCam;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    void logger(const char*);
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    QString replay;
    QList<Point> replayList;
    QTimer *timer;
cv::VideoCapture cap;
ChessboardObserver observer;

DetectionState state;
bool interrupted;


private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
void displayInMessageBox(QString,int);
void log(Point,Point);
void whoMoves(QString color);


void on_actionRestart_triggered();

void on_actionSzach_triggered();

void on_actionRoszada_kr_tka_triggered();

void on_actionBicie_w_przelocie_triggered();

void on_actionRoszada_triggered();

void on_actionPrzerwana_roszada_triggered();

void on_action1_triggered();

void on_action2_triggered();

void on_action3_triggered();

void on_action4_triggered();

void on_action5_triggered();

void on_action6_triggered();

void on_action7_triggered();

void on_action8_triggered();

void on_action9_triggered();

void on_action10_triggered();

void on_actionDomy_lna_triggered();

void on_actionZapisz_powt_rk_gry_triggered();

void on_actionWczytaj_powt_rk_triggered();


void update();

void on_h_ups_valueChanged(int value);

void on_h_lows_valueChanged(int value);

void on_s_lows_valueChanged(int value);

void on_s_ups_valueChanged(int value);

void on_v_lows_valueChanged(int value);

void on_v_ups_valueChanged(int value);

void on_gc_valueChanged(int value);

void on_fc_valueChanged(int value);

void on_wft_valueChanged(int value);

void on_dr_stateChanged(int arg1);

void on_ac_stateChanged(int arg1);

void on_dr_2_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;
    ChessCam::Chessboard* chessboard;

};

#endif // MAINWINDOW_H
