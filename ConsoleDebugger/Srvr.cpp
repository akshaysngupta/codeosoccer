/*#include "Server.h"
#include <QMainWindow>


server::server(QTextEdit *edbox):
    board(edbox)
{}

void server::receive_message()
{
    char buffer[1000];
    do
        {
        memset(buffer,0,999);
        int inDataLength=recv(SocketServer,buffer,1000,0);
        //board->append(buffer + "\n");
        }while(strcmp(buffer,"close")!=0);

         // Shutdown our socket
        shutdown(SocketServer,SD_SEND);

        // Close our socket entirely
        closesocket(SocketServer);

        // Cleanup Winsock
        WSACleanup();
        //system("PAUSE");
        board->append("close\n");
        return;
}

void server::start_server()
{
    WSADATA WsaDat;
    if(WSAStartup(MAKEWORD(2,2),&WsaDat)!=0)
    {
        board->append("WSA Initialization failed!\n");
        WSACleanup();
        //system("PAUSE");
        board->append("close\n");
        return;
    }

    SocketServer=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(SocketServer==INVALID_SOCKET)
    {
        board->append("Socket creation failed.\r\n");
        WSACleanup();
        board->append("close\n");
        return;
    }

    SOCKADDR_IN serverInf;
    serverInf.sin_family=AF_INET;
    serverInf.sin_addr.s_addr=INADDR_ANY;
    serverInf.sin_port=htons(8888);

    if(bind(SocketServer,(SOCKADDR*)(&serverInf),sizeof(serverInf))==SOCKET_ERROR)
    {
        board->append("Unable to bind socket!\r\n");
        WSACleanup();
        board->append("close\n");
        return;
    }

    listen(SocketServer,1);

    SOCKET TempSock=SOCKET_ERROR;
    while(TempSock==SOCKET_ERROR)
    {
        board->append("Waiting for incoming connections...\r\n");
        TempSock=accept(SocketServer,NULL,NULL);
    }
    SocketServer=TempSock;

    board->append("Client connected!\r\n\r\n");
    //receive_message();
    return;
}

server::~server()
{
}
*/
