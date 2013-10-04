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
#include <QPlainTextEdit>
#include <QLabel>
#pragma comment(lib,"ws2_32.lib")

class Server : public QObject
{
    Q_OBJECT

public:
    Server();
    ~Server();
    SOCKET SocketServer;
    QLineEdit *status;

    void receive_message();
signals:
    void finished();
    void receivedData(QString s);
public slots:
    void server_start();
    
};

#endif // SERVER_H
