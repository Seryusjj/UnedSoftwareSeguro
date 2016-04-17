#pragma once
#include <string>

#if _WIN32
#include <winsock2.h>
#pragma comment (lib, "Ws2_32.lib")
#else
#include <netinet/in.h>
#endif

//IPv4 maximum reassembly buffer size is 576, IPv6 raises this to 1,500
#define MAX_BUFFER_SIZE 576

class UdpConnection
{
private:
	/* Information about the server */
	struct sockaddr_in  _server;
	/* Information about the client */
	struct sockaddr_in  _client;
	/* Socket id */
	int _udpSocket;

private:
	int _clientLength;

private:
	bool preInitSocket();
	void configSocket(const int port);

public:
	UdpConnection(const int port);

	~UdpConnection();

public:
	void openSocket();
	void closeSocket();
	bool sendData(const char* data);
	bool receiveData(char* buffer);
};
