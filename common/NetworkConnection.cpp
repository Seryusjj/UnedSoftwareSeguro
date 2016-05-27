#include "NetworkConnection.h"
#include "SerializablePOD.h"

#if !_WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#endif

NetworkConnection::NetworkConnection(int port)
	: _socket(-1)
{
	_clientLength = sizeof(struct sockaddr_in);

	/* Clear out server struct */
	memset((void *)&_server, '\0', _clientLength);

	/* Set family and port */
	_server.sin_family = AF_INET;
	_server.sin_port = htons(port);
	_server.sin_addr.s_addr = htonl(INADDR_ANY);
}

NetworkConnection::NetworkConnection(int port, std::string ip)
	: _socket(-1)
{
	_serverLength = sizeof(struct sockaddr_in);

	memset(&_server, '\0', _serverLength);

	/* Set family and port of the server*/
	_server.sin_family = AF_INET;
	_server.sin_port = htons(port);
	_server.sin_addr.s_addr = inet_addr(ip.c_str());

	memset(&_client, '\0', _serverLength);

	/* Set family and port of the client*/
	_client.sin_family = AF_INET;
	_client.sin_port = htons(0);
	_client.sin_addr.s_addr = htonl(INADDR_ANY);
}

NetworkConnection::~NetworkConnection()
{
}

inline void NetworkConnection::open()
{
	//already open
	if (_socket >= 0) return;

	if (!preInitSocket()) return;
	/*Solo tcp de momento*/
	_socket = socket(AF_INET, SOCK_STREAM, 0);

	if (_socket < 0)
	{
		//"Could not create socket.");
		return;
	}
	/*Make socket reusable and nonblocking*/
	setToNonBlocking();

	/* Bind the socket */
	bindSocket();

	postSocketCreation();
}

inline void NetworkConnection::close()
{
	close(_socket);
}

inline void NetworkConnection::close(int& socket)
{
#if _WIN32
	if (!(socket < 0)) {
		closesocket(socket);
	}
	WSACleanup();
#else
	if (!socket < 0) {
		close(socket);
}
#endif
	socket = -1;
}

bool NetworkConnection::preInitSocket() {
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

inline bool NetworkConnection::sendFrom(const char * data, int len, int socket)
{
	int rc = send(socket, data, len, 0);//eq to write
	if (rc < 0)
	{
		perror("send() failed");
		close_conn = TRUE;
		return false;
	}
	return true;
}

inline bool NetworkConnection::sendData(const char * data, int len)
{
	return sendFrom(data, len, _socket);
}

inline bool NetworkConnection::sendIntFrom(int32_t data, int socket)
{
	char buffer[MAX_BUFFER_SIZE];
	SerializablePOD<int32_t>::serialize(buffer, data);
	return sendFrom(buffer, SerializablePOD<int32_t>::serializeSize(data), socket);
}

inline bool NetworkConnection::sendFloatFrom(float data, int socket)
{
	char buffer[MAX_BUFFER_SIZE];
	SerializablePOD<float>::serialize(buffer, data);
	return sendFrom(buffer, SerializablePOD<float>::serializeSize(data), socket);
}

inline bool NetworkConnection::sendIntData(const int32_t data, int len)
{
	char buffer[MAX_BUFFER_SIZE];
	SerializablePOD<int32_t>::serialize(buffer, data);
	return sendData(buffer, SerializablePOD<int32_t>::serializeSize(data));
}

inline bool NetworkConnection::sendFloatData(const float data, int len)
{
	char buffer[MAX_BUFFER_SIZE];
	SerializablePOD<float>::serialize(buffer, data);
	return sendData(buffer, SerializablePOD<float>::serializeSize(data));
}

inline int32_t  NetworkConnection::readBlockingIntData() {
	int32_t result = 0;
	char buffer[MAX_BUFFER_SIZE];
	int rc = recv(_socket, buffer, MAX_BUFFER_SIZE, 0);
	SerializablePOD<int32_t>::deserialize(buffer, result);
	return result;
}

inline int32_t  NetworkConnection::readBlockingFloatData() {
	float result = 0;
	char buffer[MAX_BUFFER_SIZE];
	int rc = recv(_socket, buffer, MAX_BUFFER_SIZE, 0);
	SerializablePOD<float>::deserialize(buffer, result);
	return result;
}