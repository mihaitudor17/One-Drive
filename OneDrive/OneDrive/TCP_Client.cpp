#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <fstream>
#include <filesystem>
#include "TCP_Client.h"
#include <chrono>
#include <thread>
#pragma comment(lib, "ws2_32.lib")
bool Client::sendUser(SOCKET clientSock, char username[FILENAME_MAX])
{
	int byRecv = send(clientSock, username, FILENAME_MAX, 0);
	if (byRecv == 0 || byRecv == -1) {
		return 0;
	}
	else
		return 1;
}
bool Client::sendFileName(SOCKET clientSock, char fileRequested[FILENAME_MAX])
{
	int byRecv = send(clientSock, fileRequested, FILENAME_MAX, 0);
	if (byRecv == 0 || byRecv == -1) {
		return 0;
	}
	else
		return 1;
}
char* Client::recvFileName(SOCKET clientSock)
{
	char fileRequested[FILENAME_MAX];
	memset(fileRequested, 0, FILENAME_MAX);
	int byRecv = recv(clientSock, fileRequested, FILENAME_MAX, 0);
	//std::cout << fileRequested << std::endl;
	std::string temp = "NULL";
	if (byRecv == 0 || byRecv == -1) {
		return &temp[0];
	}
	return fileRequested;
}
long Client::recvFileSize(SOCKET clientSock)
{
	int byRecv;
	long fileRequestedsize = 0;
	byRecv = recv(clientSock, (char*)&fileRequestedsize, sizeof(long), 0);
	//std::cout << fileRequestedsize << std::endl;
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
	std::string temp;
	int bySendinfo = 1;
	if (file.is_open()) {

		file.seekg(0, std::ios::end);
		long fileSize = file.tellg();
		if (sendFileSize(clientSock, fileSize) == -1)
			return 0;
		file.seekg(0, std::ios::beg);
		if (fileSize > 0)
		{
			do {

				file.read(bufferFile, BUFFER_SIZE);
				if (file.gcount() > 0 || temp != bufferFile)
				{
					bySendinfo = send(clientSock, bufferFile, file.gcount(), 0);
					std::this_thread::sleep_for(std::chrono::milliseconds(10));// fara sleep se vor concatena buffer-ele alta solutie ar fi trimiterea in acelasi buffer a mesajelor
					std::string temporary(bufferFile, bySendinfo);
					/*std::cout << temporary << std::endl;*/
				}
				temp = bufferFile;
				if (bySendinfo == 0 || bySendinfo == -1) {
					return 0;
				}
			} while (file.gcount() > 0);
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		file.close();
		file.close();

	}
	else {
		return 0;
	}
	return 1;
}
bool Client::writeToFile(SOCKET clientSock, std::string fullPath, int fileRequestedsize)
{
	std::ofstream file;
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
		/*std::cout << bufferFile << std::endl;*/
		if (byRecv == 0 || byRecv == -1) {
			return 0;
		}
		file.write(bufferFile, byRecv);
		fileDownloaded += byRecv;
	} while (fileDownloaded < fileRequestedsize);
	file.close();
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
	std::string temp = "127.0.0.1";
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
