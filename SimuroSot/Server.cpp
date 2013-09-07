#include "StdAfx.h"
#include "Server.h"
#include <iostream>

server::server(void)
{
	WSADATA WsaDat;
	if(WSAStartup(MAKEWORD(2,2),&WsaDat)!=0)
	{
		std::cout<<"WSA Initialization failed!\r\n";
		WSACleanup();
		system("PAUSE");
		exit(1);
	}
	
	SocketServer=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(SocketServer==INVALID_SOCKET)
	{
		std::cout<<"Socket creation failed.\r\n";
		WSACleanup();
		system("PAUSE");
		exit(1);
	}
	
	SOCKADDR_IN serverInf;
	serverInf.sin_family=AF_INET;
	serverInf.sin_addr.s_addr=INADDR_ANY;
	serverInf.sin_port=htons(8888);

	if(bind(SocketServer,(SOCKADDR*)(&serverInf),sizeof(serverInf))==SOCKET_ERROR)
	{
		std::cout<<"Unable to bind socket!\r\n";
		WSACleanup();
		system("PAUSE");
		exit(1);
	}

	listen(SocketServer,1);

	SOCKET TempSock=SOCKET_ERROR;
	while(TempSock==SOCKET_ERROR)
	{
		std::cout<<"Waiting for incoming connections...\r\n";
		TempSock=accept(SocketServer,NULL,NULL);
	}
	SocketServer=TempSock;
		
	std::cout<<"Client connected!\r\n\r\n";
	
}
void server::RecieveMessage()
{
	char buffer[1000];
	do
		{
		memset(buffer,0,999);
		int inDataLength=recv(SocketServer,buffer,1000,0);
		std::cout<<buffer<<"\n";
		}while(strcmp(buffer,"close")!=0);

		 // Shutdown our socket
		shutdown(SocketServer,SD_SEND);

		// Close our socket entirely
		closesocket(SocketServer);

		// Cleanup Winsock
		WSACleanup();
		system("PAUSE");
		exit(1);
}