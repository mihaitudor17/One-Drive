#pragma once
#include <iostream>
#include <fstream>
#include <ws2tcpip.h>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#define BUFFER_SIZE 1024
class Server
{
private:
	SOCKET clientSock;
public:
	bool connectClient();
	char* recvFileName(SOCKET clientSock);
	bool sendFileName(SOCKET clientSock, char fileRequested[FILENAME_MAX]);
	long recvFileSize(SOCKET clientSock);
	bool sendFileSize(SOCKET clientSock, long fileSize);
	bool sendFile(SOCKET clientSock, std::string path);
	bool writeToFile(SOCKET clientSock, std::string fullPath, int fileRequestedsize);
	char* recvUser(SOCKET clientSock);
	Server() {}
	SOCKET getSock()
	{
		return clientSock;
	}
	void setSock(SOCKET clientSock)
	{
		this->clientSock = clientSock;
	}
};