#include "Server.h"
bool Server::connectClient()
{
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);
	if (WSAStartup(ver, &wsData) != 0) {
		std::cerr << "Error starting winsock!" << std::endl;
		return -1;
	}
	sockaddr_in listenerHint;
	listenerHint.sin_family = AF_INET;
	listenerHint.sin_port = htons(55000);
	listenerHint.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(listenerSock, (sockaddr*)&listenerHint, sizeof(listenerHint));
	listen(listenerSock, SOMAXCONN);

	sockaddr_in clientHint;
	int clientSize = sizeof(clientHint);

	SOCKET clientSock = accept(listenerSock, (sockaddr*)&clientHint, &clientSize);

	if (clientSock == SOCKET_ERROR) {
		std::cerr << "Error accept socket! " << WSAGetLastError() << std::endl;
		closesocket(listenerSock);
		WSACleanup();
		return -1;
	}

	char host[NI_MAXHOST];
	char serv[NI_MAXSERV];

	if (getnameinfo((sockaddr*)&clientHint, sizeof(clientHint), host, NI_MAXHOST, serv, NI_MAXSERV, 0) == 0) {
		std::cout << "Host: " << host << " connected on port: " << serv << std::endl;
	}
	else {
		inet_ntop(AF_INET, &clientHint.sin_addr, host, NI_MAXHOST);
		std::cout << "Host: " << host << " connected on port: " << ntohs(clientHint.sin_port) << std::endl;
	}
	closesocket(listenerSock);
	this->clientSock = clientSock;
}
char* Server::recvFileName(SOCKET clientSock)
{
	char fileRequested[FILENAME_MAX];
	memset(fileRequested, 0, FILENAME_MAX);
	int byRecv = recv(clientSock, fileRequested, FILENAME_MAX, 0);

	if (byRecv == 0 || byRecv == -1) {

		return 0;
	}
	return fileRequested;
}
bool Server::sendFileName(SOCKET clientSock, char fileRequested[FILENAME_MAX])
{
	int byRecv = send(clientSock, fileRequested, FILENAME_MAX, 0);
	std::cout << fileRequested << std::endl;
	if (byRecv == 0 || byRecv == -1) {
		return 0;
	}
	else
		return 1;
}
long Server::recvFileSize(SOCKET clientSock)
{
	int byRecv;
	long fileRequestedsize = 0;
	byRecv = recv(clientSock, (char*)&fileRequestedsize, sizeof(long), 0);
	if (byRecv == 0 || byRecv == -1)
		return -1;
	return fileRequestedsize;
}
bool Server::sendFileSize(SOCKET clientSock, long fileSize)
{
	int bySendinfo = send(clientSock, (char*)&fileSize, sizeof(long), 0);
	if (bySendinfo == 0 || bySendinfo == -1) {

		return 0;
	}
	return 1;
}
bool Server::sendFile(SOCKET clientSock, std::string path)
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
bool Server::writeToFile(SOCKET clientSock, std::ofstream file, std::string fullPath, int fileRequestedsize)
{
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
		if (byRecv == 0 || byRecv == -1) {
			return 0;
		}
		file.write(bufferFile, byRecv);
		fileDownloaded += byRecv;
	} while (fileDownloaded < fileRequestedsize);
	file.close();
	return 1;
}
char* Server::recvUser(SOCKET clientSock)
{
	char username[FILENAME_MAX];
	memset(username, 0, FILENAME_MAX);
	int byRecv = recv(clientSock, username, FILENAME_MAX, 0);

	if (byRecv == 0 || byRecv == -1) {

		return 0;
	}
	return username;
}