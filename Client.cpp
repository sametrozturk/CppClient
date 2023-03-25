#include <iostream>
#include <winsock2.h>
#include <Ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main() {

	WSADATA wsaData;
	const char* server_ip = "127.0.0.1";
	const int server_port = 8080;

	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		std::cout << "WSAStartup failed with error: " << iResult << std::endl;
		return 1;
	}

	SOCKET ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ConnectSocket == INVALID_SOCKET) {
		std::cout << "socket failed with error: " << WSAGetLastError() << std::endl;
		WSACleanup();
		return 1;
	}

	sockaddr_in serverAddress;
	inet_pton(AF_INET, server_ip, &serverAddress.sin_addr);
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(server_port);

	iResult = connect(ConnectSocket, (sockaddr*)&serverAddress, sizeof(serverAddress));
	if (iResult == SOCKET_ERROR) {
		std::cout << "connect failed with error: " << WSAGetLastError() << std::endl;
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	const char* message = "Hello, server!";
	int messageLength = strlen(message);
	iResult = send(ConnectSocket, message, messageLength, 0);
	if (iResult == SOCKET_ERROR) {
		std::cout << "send failed with error: " << WSAGetLastError() << std::endl;
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	std::cout << "Sent message to server: " << message << std::endl;

	char recvbuf[1024];
	iResult = recv(ConnectSocket, recvbuf, sizeof(recvbuf), 0);
	if (iResult > 0) {
		recvbuf[iResult] = '\0';
		std::cout << "Received message from server: " << recvbuf << std::endl;
	}
	else if (iResult == 0) {
		std::cout << "Connection closed by server." << std::endl;
	}
	else {
		std::cout << "recv failed with error: " << WSAGetLastError() << std::endl;
	}

	// Clean up
	closesocket(ConnectSocket);
	WSACleanup();

	return 0;
}