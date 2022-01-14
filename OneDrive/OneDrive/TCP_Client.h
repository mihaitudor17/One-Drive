#pragma once
#define WIN32_LEAN_AND_MEAN
#include <fstream>
#include <string>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <filesystem>
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
#define BUFFER_SIZE 1024
class Client {
public:
	Client();
	SOCKET getSock()
	{
		return clientSock;
	}
	void setSock(SOCKET tempSock)
	{
		clientSock = tempSock;
	}
	bool sendFileName(SOCKET clientSock, char fileRequested[FILENAME_MAX]);
	char* recvFileName(SOCKET clientSock);
	long recvFileSize(SOCKET clientSock);
	bool sendFileSize(SOCKET clientSock, long fileRequestedsize);
	bool writeToFile(SOCKET clientSock, std::string fullPath, int fileRequestedsize);
	bool sendFile(SOCKET clientSock, std::string path);
	bool sendUser(SOCKET clientSock, char username[FILENAME_MAX]);
	bool connectServer();
	bool clientClose = false;
private:
	SOCKET clientSock;
};


