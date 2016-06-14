#pragma once

#define FD_SETSIZE 1000

#if _WIN32
#include <winsock2.h>
#pragma comment (lib, "Ws2_32.lib")
#else
#include <netinet/in.h>
#endif

#include "ISerializable.h"
#include <string>
#include <map>
#include "DLLDefines.h"

//IPv4 maximum reassembly buffer size is 576, IPv6 raises this to 1,500
#define MAX_BUFFER_SIZE 576

/*
Abstrac socket communication class
*/
class EXPORT NetworkConnection
{
private:
	NetworkConnection() {};
protected:
	int     on = 1;
	int    close_conn;

	bool preInitSocket();

	int _clientLength;
	int _serverLength;
	/* Information about the server */
	struct sockaddr_in  _server;
	/* Information about the client */
	struct sockaddr_in  _client;
	/* Socket id */
	int _socket;

	/* Allow socket descriptor to be reuseable and nonblocking */
	virtual void setToNonBlocking() = 0;
	virtual bool bindSocket() = 0;
	virtual void  postSocketCreation() = 0;

	void close(int& _socket);
	/*Constructor for server just specify the port to listen on*/
	NetworkConnection(int port);

	/*Constructor for client must specify the port and ip of the server*/
	NetworkConnection(int port, std::string ip);
public:

	~NetworkConnection();

	virtual void open();
	void close();
	int getSocketNumber() { return _socket; }

	template <typename T>
	T*  readBlocking() {
		char buffer[MAX_BUFFER_SIZE];
		int rc = recv(_socket, buffer, MAX_BUFFER_SIZE, 0);
		T *s = new T();		s->deserialize(buffer);
		return s;
	}

	inline int32_t  readBlockingIntData();
	inline int32_t  readBlockingIntDataFrom(int socket);

	inline int32_t  readBlockingFloatData();

	virtual bool sendData(const char* data, int len);

	/*Send data from the passed socket*/
	virtual bool sendFrom(const char* data, int len, int socket);

	inline bool sendIntFrom(int32_t data, int socket);

	inline bool sendFloatFrom(float data, int socket);

	inline bool sendIntData(int32_t data);

	inline bool sendFloatData(const float data);
};
