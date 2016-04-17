#pragma once
#include <string>

#if _WIN32
#include <winsock2.h>
#pragma comment (lib, "Ws2_32.lib")
#else
#include <netinet/in.h>
#endif

//IPv4 maximum reassembly buffer size is 576
#define BUFFER_SIZE 570

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
