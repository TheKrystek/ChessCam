#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "Chessboard.h"
#include <QMainWindow>

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

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
void displayInMessageBox(QString,int);
void log(QString);


private:
    Ui::MainWindow *ui;
    ChessCam::Chessboard* chessboard;

};

#endif // MAINWINDOW_H
