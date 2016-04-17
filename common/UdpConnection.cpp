#include "UdpConnection.h"

#if !_WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#endif

UdpConnection::UdpConnection(const int port) :
	_udpSocket(-1) {
	configSocket(port);
}

UdpConnection::~UdpConnection()
{
	closeSocket();
}

bool UdpConnection::preInitSocket() {
#if _WIN32
	/* Used to open Windows connection */
	WSADATA w;
	/* Open windows connection */
	if (WSAStartup(MAKEWORD(2, 2), &w) != 0)
	{
		//"Could not open Windows connection.");
		return false;
	}
#endif

	return true;
}

void UdpConnection::openSocket() {
	/*Pre initialize some parameters that able to open the socket*/
	if (!preInitSocket()) { return; }

	/* Open a datagram socket */
	_udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (_udpSocket < 0)
	{
		//"Could not create socket.");
		closeSocket();
		return;
	}

	/* Bind local address to socket */
	if (bind(_udpSocket, (struct sockaddr *)&_server, sizeof(struct sockaddr_in)) < 0)
	{
		//"Cannot bind address to socket.");
		closeSocket();
		return;
	}
}

void UdpConnection::closeSocket() {
#if _WIN32
	if (!(_udpSocket < 0)) {
		closesocket(_udpSocket);
	}
	WSACleanup();
#else
	if (!_udpSocket < 0) {
		close(_udpSocket);
	}
#endif
	_udpSocket = -1;
}

void UdpConnection::configSocket(const int port) {
	_clientLength = sizeof(struct sockaddr_in);

	/* Clear out server struct */
	memset((void *)&_server, '\0', sizeof(struct sockaddr_in));

	/* Set family and port */
	_server.sin_family = AF_INET;
	_server.sin_port = htons(4000);
	_server.sin_addr.s_addr = htonl(INADDR_ANY);
}

bool UdpConnection::sendData(const char* data) {
	int sended = sendto(_udpSocket, data, BUFFER_SIZE, 0, (struct sockaddr *)&_client, _clientLength);
	if (sended <= 0) {
		//error sending data
		closeSocket();
		return false;
	}
	return true;
}

bool UdpConnection::receiveData(char* buffer) {
#if _WIN32
	if (recvfrom(_udpSocket, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&_client, &_clientLength) <= 0)
	{
		//error receiving data
		closeSocket();
		return false;
	}
#else
	if (recvfrom(_udpSocket, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&_client, (socklen_t*)&_clientLength) < 0)
	{
		//error receiving data
		closeSocket();
		return false;
	}
#endif
	return true;
}