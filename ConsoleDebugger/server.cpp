#include "server.h"

Server::Server()
{
}

Server::~Server()
{
}

void Server::receive_message()
{
    char buffer[1000];
    QString message;
    do
    {
        memset(buffer,0,999);
        int inDataLength=recv(SocketServer,buffer,1000,0);
        message=buffer;
        emit receivedData(QString(message));
    }while(strcmp(buffer,"close")!=0);

     // Shutdown our socket
    shutdown(SocketServer,SD_SEND);

    // Close our socket entirely
    closesocket(SocketServer);

    // Cleanup Winsock
    WSACleanup();
    status->setText("close\n");

    return;
}

void Server::server_start()
{
    WSADATA WsaDat;
    if(WSAStartup(MAKEWORD(2,2),&WsaDat)!=0)
    {
        status->setText("WSA Initialization failed!\n");
        WSACleanup();
        status->setText("close\n");
        emit finished();
        return;
    }

    SocketServer=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(SocketServer==INVALID_SOCKET)
    {
        status->setText("Socket creation failed.\r\n");
        WSACleanup();
        status->setText("close\n");
        emit finished();
        return;
    }

    SOCKADDR_IN serverInf;
    serverInf.sin_family=AF_INET;
    serverInf.sin_addr.s_addr=INADDR_ANY;
    serverInf.sin_port=htons(8888);

    if(bind(SocketServer,(SOCKADDR*)(&serverInf),sizeof(serverInf))==SOCKET_ERROR)
    {
        status->setText("Unable to bind socket!\r\n");
        WSACleanup();
        status->setText("close\n");
        emit finished();
        return;
    }

    listen(SocketServer,1);


    SOCKET TempSock=SOCKET_ERROR;
    while(TempSock==SOCKET_ERROR)
    {
        status->setText("Waiting for incoming connections...\r\n");
        TempSock=accept(SocketServer,NULL,NULL);
    }
    SocketServer=TempSock;

    status->setText("Client connected!\r\n\r\n");

    receive_message();
}
