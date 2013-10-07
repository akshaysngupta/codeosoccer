#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <QProcess>
#include <QThread>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void simulator();
    QProcess *p;
    Server *s;
    QThread *thread;

    
private slots:
    void on_start_strat_clicked();
    void on_exit_strat_clicked();
    void onReceivedData(QString message);
    void on_blueButton_toggled(bool checked);
    void onReadReady();
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
