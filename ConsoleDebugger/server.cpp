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
    int j,k,count=0;
    QString message,data,POSB,POS0,POS1,POS2,POS3,VELB,VEL0,VEL1,VEL2,VEL3,ANG0,ANG1,ANG2,ANG3;
    do
    {
        memset(buffer,0,999);
        int inDataLength=recv(SocketServer,buffer,1000,0);
        message=buffer;
        data.clear();
        for(int l=0;l<message.length();l++)
        {
            if(message[l]=='@')
            {
                count++;
                continue;
            }
            if(count==3)
            {

                POSB.clear();
                POS0.clear();
                POS1.clear();
                POS2.clear();
                POS3.clear();
                VELB.clear();
                VEL0.clear();
                VEL1.clear();
                VEL2.clear();
                VEL3.clear();
                ANG0.clear();
                ANG1.clear();
                ANG2.clear();
                ANG3.clear();

                j=0;
                k=0;

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
                    else if(j==4)
                    {
                        if(k==0)
                            POS3.append(message[i]);
                        else if(k==1)
                            VEL3.append(message[i]);
                        else
                            ANG3.append(message[i]);
                    }
                }
                posb->setText(POSB);
                pos0->setText(POS0);
                pos1->setText(POS1);
                pos2->setText(POS2);
                pos3->setText(POS3);
                velb->setText(VELB);
                vel0->setText(VEL0);
                vel1->setText(VEL1);
                vel2->setText(VEL2);
                vel3->setText(VEL3);
                ang0->setText(ANG0);
                ang1->setText(ANG1);
                ang2->setText(ANG2);
                ang3->setText(ANG3);
                count=0;
            }
            else
            {
                data.append(message[l]);
            }
        }
        if (random->toPlainText().length() > 1000)
        {
            QString text = random->toPlainText();
            text = text[random->toPlainText().length() - 1000];
            random->setText(text); // Reset text

            // This code just resets the cursor back to the end position
            // If you don't use this, it moves back to the beginning.
            // This is helpful for really long text edits where you might
            // lose your place.
            QTextCursor cursor = random->textCursor();
            cursor.setPosition(random->document()->characterCount() - 1);
            random->setTextCursor(cursor);
        }
        random->append(data);

    }while(strcmp(buffer,"close")!=0);

     // Shutdown our socket
    shutdown(SocketServer,SD_SEND);

    // Close our socket entirely
    closesocket(SocketServer);

    // Cleanup Winsock
    WSACleanup();
    //system("PAUSE");
    status->setText("close\n");

    return;
}

void Server::server_start()
{
    WSADATA WsaDat;
    if(WSAStartup(MAKEWORD(2,2),&WsaDat)!=0)
    {
        //qDebug("0");
        status->setText("WSA Initialization failed!\n");
        WSACleanup();
        status->setText("close\n");
        emit finished();
        return;
    }

    SocketServer=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(SocketServer==INVALID_SOCKET)
    {
        //qDebug("1");
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
        //qDebug("2");
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
        //qDebug("3");
        status->setText("Waiting for incoming connections...\r\n");
        TempSock=accept(SocketServer,NULL,NULL);
    }
    SocketServer=TempSock;

    status->setText("Client connected!\r\n\r\n");

    receive_message();
}
