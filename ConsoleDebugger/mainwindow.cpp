#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <Windows.h>
#include <fstream>
#include <QPixmap>
using namespace std;

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    thread = new QThread();
    s = new Server();
    s->status=ui->status;

    s->moveToThread(thread);
    p = new QProcess();
    connect(thread, SIGNAL(started()), s, SLOT(server_start()));
    connect(s, SIGNAL(receivedData(QString)), this, SLOT(onReceivedData(QString)));
    connect(s, SIGNAL(finished()), thread, SLOT(quit()));
    connect(p, SIGNAL(finished(int)), thread, SLOT(quit()));
    connect(p, SIGNAL(finished(int)), this, SLOT(on_exit_strat_clicked()));
//    connect(p, SIGNAL(finished(int), this, SLOT(on_exit_strat_clicked()));
    ui->bot0->setPixmap(QPixmap(":/new/images/B1s.jpg"));
    ui->bot1->setPixmap(QPixmap(":/new/images/b2s.jpg"));
    ui->bot2->setPixmap(QPixmap(":/new/images/b3s.jpg"));
    TeamColorFromUI = 0;
    TeamColorFromStartegy = 0;
    //for message parsing
    ui->random->setMaximumBlockCount(200);
    ui->random->ensureCursorVisible();
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
    p->kill();
    QApplication::quit();
}

void MainWindow::onReceivedData(QString message)
{
    QString data,POSB,POS0,POS1,POS2,VELB,VEL0,VEL1,VEL2,ANG0,ANG1,ANG2;
    data.clear();
    int j,k,count=0;
    qDebug(message.toStdString().c_str());
    qDebug("\n");
    for(int l=0;l<message.length();l++)
    {
        qDebug("l %d\n",l);
        if(message[l]=='@')
        {
            count++;
            if(count==3)
            {
                l++;
                TeamColorFromStartegy = message[l].digitValue();
            }
            continue;
        }
        if(count==4)
        {

            POSB.clear();
            POS0.clear();
            POS1.clear();
            POS2.clear();
            VELB.clear();
            VEL0.clear();
            VEL1.clear();
            VEL2.clear();
            ANG0.clear();
            ANG1.clear();
            ANG2.clear();

            j=0;
            k=0;

            if(TeamColorFromStartegy != TeamColorFromUI)
                skipThrough(message,l);

            for(int i=l;i<message.length();i++)
            {
                if(message[i]=='*')
                {
                    j++;
                    continue;
                }
                if(message[i]=='@')
                {
                    k++;
                    j=0;
                    if(k==2)
                        j=1;
                    if(k==3)
                    {
                        l=l+i-1;
                        break;
                    }
                    continue;
                }
                if(j==0)
                {
                    if(k==0)
                        POSB.append(message[i]);
                    else if(k==1)
                        VELB.append(message[i]);
                }
                else if(j==1)
                {
                    if(k==0)
                        POS0.append(message[i]);
                    else if(k==1)
                        VEL0.append(message[i]);
                    else
                        ANG0.append(message[i]);
                }
                else if(j==2)
                {
                    if(k==0)
                        POS1.append(message[i]);
                    else if(k==1)
                        VEL1.append(message[i]);
                    else
                        ANG1.append(message[i]);
                }
                else if(j==3)
                {
                    if(k==0)
                        POS2.append(message[i]);
                    else if(k==1)
                        VEL2.append(message[i]);
                    else
                        ANG2.append(message[i]);
                }
            }
            ui->posb->setText(POSB);
            ui->pos0->setText(POS0);
            ui->pos1->setText(POS1);
            ui->pos2->setText(POS2);
            ui->velb->setText(VELB);
            ui->vel0->setText(VEL0);
            ui->vel1->setText(VEL1);
            ui->vel2->setText(VEL2);
            ui->ang0->setText(ANG0);
            ui->ang1->setText(ANG1);
            ui->ang2->setText(ANG2);
            count=0;

            if(TeamColorFromStartegy == TeamColorFromUI)
            {
                skipThrough(message,l);
                l--;
            }
        }
        else
        {
            data.append(message[l]);
        }
    }

    ui->random->appendPlainText(data);
}

void MainWindow::skipThrough(QString message,int &l)
{
    int count = 0;
    while(count!=3)
    {
        if(message[l]=='@')
            count++;
        l++;
    }
}

void MainWindow::simulator()
{    
    if(!p->isOpen())
    {
        p->kill();
        p->start("RobotSoccer.exe");
    }
    ui->status->setText("Starting Simulator");
}

void MainWindow::on_blueButton_toggled(bool checked)
{

    if(checked) {
        TeamColorFromUI = 0;
        ui->bot0->setPixmap(QPixmap(":/new/images/B1s.jpg"));
        ui->bot1->setPixmap(QPixmap(":/new/images/b2s.jpg"));
        ui->bot2->setPixmap(QPixmap(":/new/images/b3s.jpg"));
    } else {
        TeamColorFromUI = 1;
        ui->bot0->setPixmap(QPixmap(":/new/images/y1s.jpg"));
        ui->bot1->setPixmap(QPixmap(":/new/images/y2s.jpg"));
        ui->bot2->setPixmap(QPixmap(":/new/images/y3s.jpg"));
    }
}
