#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

using namespace std;

int main() {
    WSADATA wsaData;
    int wsaerr;
    WORD wVersionRequested = MAKEWORD(2, 2);

    wsaerr = WSAStartup(wVersionRequested, &wsaData);
    if (wsaerr != 0) {
        cout << "WSAStartup failed with error: " << wsaerr << endl;
        return 1;
    }

    cout << "The Winsock DLL found!" << endl;
    cout << "The Status: " << wsaData.szSystemStatus << endl;

    // Create a socket
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        cout << "Error at socket(): " << WSAGetLastError() << endl;
        WSACleanup();
        return 1;  // Changed the return value to 1 on error
    }
    else {
        cout << "socket() is good" << endl;
    }

    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    InetPton(AF_INET, L"127.0.0.1", &clientService.sin_addr.s_addr);
    clientService.sin_port = htons(8080);

    if (connect(clientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
        cout << "Client: Connect() - Failed to connect." << endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;  // Return 1 on failure
    }
    else {
        cout << "Client: connect() is good." << endl;
        cout << "Client: Can start sending and receiving data..." << endl;
    }

    char buffer[200];
    printf("Enter your Message ");
    cin.getline(buffer, 200);
    int byteCount = send(clientSocket, buffer, 200, 0);
    if (byteCount == SOCKET_ERROR) {
        printf("Client send error %ld.\n", WSAGetLastError());  // Changed "Server" to "Client"
        return -1;
    }
    else {
        printf("Client: sent %d bytes \n", byteCount);  // Changed "byteCount" to "%d"
    }

    // Receiving

    char receiveBuffer[200] = "";
    byteCount = recv(clientSocket, receiveBuffer, 200, 0);  // Changed "acceptSocket" to "clientSocket"
    if (byteCount < 0) {
        printf("Client: error %ld.\n", WSAGetLastError());
        return 0;
    }
    else {
        printf("Received data: %s \n", receiveBuffer);
    }
    WSACleanup(); // Cleanup Winsock when done
    return 0;
}
