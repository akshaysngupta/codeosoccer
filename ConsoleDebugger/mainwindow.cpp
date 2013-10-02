#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <Windows.h>
#include <fstream>

using namespace std;

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    thread = new QThread();
    s = new Server();

    s->posb = ui->posb;
    s->pos0 = ui->pos0;
    s->pos1 = ui->pos1;
    s->pos2 = ui->pos2;
    s->pos3 = ui->pos3;

    s->velb = ui->velb;
    s->vel0 = ui->vel0;
    s->vel1 = ui->vel1;
    s->vel2 = ui->vel2;
    s->vel3 = ui->vel3;

    s->ang0 = ui->ang0;
    s->ang1 = ui->ang1;
    s->ang2 = ui->ang2;
    s->ang3 = ui->ang3;

    s->random = ui->random;
    s->status = ui->status;

    s->moveToThread(thread);
    connect(thread, SIGNAL(started()), s, SLOT(server_start()));
    connect(s, SIGNAL(finished()), thread, SLOT(quit()));
}

MainWindow::~MainWindow()
{
    delete thread;
    delete s;
    delete ui;
}

void MainWindow::on_start_strat_clicked()
{
    if(!thread->isRunning())
    {
        simulator();
        thread->start();
    }
}

void MainWindow::on_exit_strat_clicked()
{
    ui->status->setText("Exiting Simulator");
    p.kill();
    QApplication::quit();
}

void MainWindow::simulator()
{
    p.start("RobotSoccer.exe");
    if(!p.isOpen())
    {
        p.kill();
        p.start("RobotSoccer.exe");
    }
    ui->status->setText("Starting Simulator");
}
