#include "TCP_Client.h"
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
    std::ifstream f("input.txt");
    std::string temp;
    while(std::getline(f, temp))
    {
        sendbuf = temp.c_str();
        send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        std::string s(recvbuf, iResult);
    }
    iResult = shutdown(ConnectSocket, SD_SEND);
    closesocket(ConnectSocket);
    WSACleanup();
    return 0;
}