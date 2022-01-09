#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <fstream>
#include <filesystem>
#include "TCP_Client.h"
#pragma comment(lib, "ws2_32.lib")
bool Client::sendFileName(SOCKET clientSock, char fileRequested[FILENAME_MAX])
{
	int byRecv = send(clientSock, fileRequested, FILENAME_MAX, 0);
	if (byRecv == 0 || byRecv == -1) {
		return 0;
	}
	else
		return 1;
}
char* Client::recvFileName(SOCKET clientSock )
{
	char fileRequested[FILENAME_MAX];
	memset(fileRequested, 0, FILENAME_MAX);
	int byRecv = recv(clientSock, fileRequested, FILENAME_MAX, 0);
	if (byRecv == 0 || byRecv == -1) {
		return NULL;
	}
	return fileRequested;
}
long Client::recvFileSize(SOCKET clientSock)
{
	int byRecv;
	long fileRequestedsize = 0;
	byRecv = recv(clientSock, (char*)&fileRequestedsize, sizeof(long), 0);
	if (byRecv == 0 || byRecv == -1)
		return -1;
	return fileRequestedsize;
}
bool Client::sendFileSize(SOCKET clientSock, long fileRequestedsize)
{
	int byRecv = send(clientSock, (char*)&fileRequestedsize, sizeof(int), 0);
	if (byRecv == 0 || byRecv == -1) {

		return 0;
	}
	return 1;
}
bool Client::sendFile(SOCKET clientSock, std::string path)
{

	std::ifstream file;
	file.open(path, std::ios::binary);
	char bufferFile[BUFFER_SIZE];
	const int fileAvailable = 200;
	const int fileNotfound = 404;
	int bySendinfo = 1;
	if (file.is_open()) {

		/*int bySendinfo = send(clientSock, (char*)&fileAvailable, sizeof(int), 0);
		if (bySendinfo == 0 || bySendinfo == -1) {

			return 0;
		}*/
		file.seekg(0, std::ios::end);
		long fileSize = file.tellg();
		if (!sendFileSize(clientSock, fileSize))
			return 0;
		file.seekg(0, std::ios::beg);
		do {

			file.read(bufferFile, BUFFER_SIZE);
			if (file.gcount() > 0)
				bySendinfo = send(clientSock, bufferFile, file.gcount(), 0);
			std::cout << bufferFile << std::endl;
			if (bySendinfo == 0 || bySendinfo == -1) {

				return 0;
			}
		} while (file.gcount() > 0);
		file.close();
	}
	else {
		/*int bySendCode = send(clientSock, (char*)&fileNotfound, sizeof(int), 0);
		if (bySendCode == 0 || bySendCode == -1) */
		return 0;
	}
	return 1;
}
bool Client::writeToFile(SOCKET clientSock, std::string fullPath, int fileRequestedsize)
{
	std::ofstream file;
	int ok = 0;
	int byRecv;
	char bufferFile[BUFFER_SIZE];
	int fileDownloaded = 0;
	file.open(fullPath, std::ios::binary | std::ios::trunc);
	if (!file.is_open())
	{
		std::ofstream file(fullPath);
		file.open(fullPath, std::ios::binary | std::ios::trunc);
	}
	do {
		memset(bufferFile, 0, BUFFER_SIZE);
		byRecv = recv(clientSock, bufferFile, BUFFER_SIZE, 0);
		if(strstr(bufferFile, "/eof"))
			ok = 1;
		if (byRecv == 0 || byRecv == -1) {
			return 0;
		}
		if (byRecv > fileRequestedsize)
			byRecv = fileRequestedsize - fileDownloaded;
		/*std::string turc(bufferfile, byrecv);
		std::cout << turc << std::endl;*/
		std::cout << bufferFile << std::endl;
		file.write(bufferFile, byRecv);
		fileDownloaded += byRecv;
	} while (fileDownloaded < fileRequestedsize);
	file.close();
	if (ok == 1)
		return 0;
	else
		return 1;
}
bool Client::connectServer() {
	
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);
	if (WSAStartup(ver, &wsData) != 0) {
		std::cerr << "Error starting winsock!" << std::endl;
		return 0;
	}
	SOCKET clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSock == INVALID_SOCKET) {
		std::cerr << "Error creating socket!, " << WSAGetLastError() << std::endl;
		return 0;
	}
	char serverAddress[NI_MAXHOST];
	memset(serverAddress, 0, NI_MAXHOST);
	std::string temp="127.0.0.1";
	strcpy(serverAddress, temp.c_str());
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(55000);
	inet_pton(AF_INET, serverAddress, &hint.sin_addr);
	if (connect(clientSock, (sockaddr*)&hint, sizeof(hint)) == SOCKET_ERROR) {
		std::cerr << "Error connect to server!, " << WSAGetLastError() << std::endl;
		closesocket(clientSock);
		WSACleanup();
		return 0;
	}
	else
	{
		this->clientSock = clientSock;
		
	}
	return 1;
}
Client::Client()
{
}
bool Client::sendUser(SOCKET clientSock, char username[FILENAME_MAX])
{
	int byRecv = send(clientSock, username, FILENAME_MAX, 0);
	if (byRecv == 0 || byRecv == -1) {
		return 0;
	}
	else
		return 1;
}