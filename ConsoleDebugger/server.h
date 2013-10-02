#ifndef SERVER_H
#define SERVER_H

#pragma once
#include <winsock2.h>
#include <QString>
#include <QMainWindow>
#include <iostream>
#include <QObject>
#include <QLineEdit>
#include <QTextEdit>
#pragma comment(lib,"ws2_32.lib")

class Server : public QObject
{
    Q_OBJECT

public:
    Server();
    ~Server();
    SOCKET SocketServer;
    void receive_message();
    QLineEdit  *status;
    QLineEdit *posb;
    QLineEdit *pos0;
    QLineEdit *pos1;
    QLineEdit *pos2;
    QLineEdit *pos3;
    QLineEdit *velb;
    QLineEdit *vel0;
    QLineEdit *vel1;
    QLineEdit *vel2;
    QLineEdit *vel3;
    QLineEdit *ang0;
    QLineEdit *ang1;
    QLineEdit *ang2;
    QLineEdit *ang3;
    QTextEdit *random;
signals:
    void finished();
    
public slots:
    void server_start();
    
};

#endif // SERVER_H
