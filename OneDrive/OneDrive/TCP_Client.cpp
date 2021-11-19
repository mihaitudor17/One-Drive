#include "TCP_Client.h"
#include <filesystem>
#include <iostream>
namespace fs = std::filesystem;
fs::path pathToShow{"./Synchronized Folder 1"};
int __cdecl client(int argc, char** argv)
{
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo* result = NULL,
        * ptr = NULL,
        hints;
    const char* sendbuf = "testing if this works";
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }
    freeaddrinfo(result);
    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }
    for (const auto& entry : fs::directory_iterator(pathToShow)) {
        const auto filenameStr = entry.path().filename().string();
        if (entry.is_regular_file()) {
            std::ifstream f(entry.path().string());
            std::string temp;
            temp = filenameStr;
            sendbuf = temp.c_str();
            send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
            iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
            while (std::getline(f, temp))
            {
                sendbuf = temp.c_str();
                send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
                iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
            }
            temp = "/eof";
            sendbuf = temp.c_str();
            send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
            iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        }
    }
    iResult = shutdown(ConnectSocket, SD_SEND);
    closesocket(ConnectSocket);
    WSACleanup();
    return 0;
}