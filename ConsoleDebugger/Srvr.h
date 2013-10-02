/*//#pragma once
#include <winsock2.h>
#include <QString>
#include <QMainWindow>
#include <iostream>
#include <QObject>
#include <QTextEdit>
//#pragma comment(lib,"ws2_32.lib")

class server : public QObject
{
    Q_OBJECT
public:
    server();
    ~server();
    SOCKET SocketServer;
    void receive_message();
    QTextEdit *board;

public slots:
    void start_server();

signals:
    void finished();
};
*/
