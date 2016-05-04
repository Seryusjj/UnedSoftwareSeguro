#pragma once

#define FD_SETSIZE 1000

#include "DLLDefines.h"
#include "ISerializable.h"
#include <string>
#include <map>
#if _WIN32
#include <winsock2.h>
#pragma comment (lib, "Ws2_32.lib")
#else
#include <netinet/in.h>
#endif

//IPv4 maximum reassembly buffer size is 576, IPv6 raises this to 1,500
#define MAX_BUFFER_SIZE 576

/*
Abstrac socket communication class
*/
class NetworkConnection
{
protected:
	int    i, len, rc, on = 1;
	int     max_sd, new_sd;

	int    close_conn;

	struct timeval       timeout;
	struct fd_set        master_set, working_set;
protected:
	/* Allow socket descriptor to be reuseable and nonblocking */
	virtual void setToNonBlocking();
	bool preInitSocket();

	unsigned int _maxConnections;
	int _clientLength;
	int _serverLength;
	/* Information about the server */
	struct sockaddr_in  _server;
	/* Information about the client */
	struct sockaddr_in  _client;
	/* Socket id */
	int _socket;
	virtual bool bindSocket() = 0;
	void virtual postSocketCreation() = 0;
	//map of client sockets and data of those sockets

	void close(int& _socket);
public:

	/*Constructor for server just specify the port to listen on*/
	NetworkConnection(int port);

	/*Constructor for client must specify the port and ip of the server*/
	NetworkConnection(int port, std::string ip);
	~NetworkConnection();
	virtual void open();

	void close();
	int getSocketNumber() { return _socket; }
	/*Return the sockaddr_in from where we read*/
	template <typename T>
	std::map<int, T*> readAsync();

	template <typename T>
	T*  readBlocking() {
		char buffer[MAX_BUFFER_SIZE];
		int rc = recv(client->getSocketNumber(), buffer, MAX_BUFFER_SIZE, 0);
		T *s = new T();
		s->deserialize(buffer);
		return s;
	}
	virtual bool sendData(const char* data, int len);
	virtual bool sendTo(const char* data, int len, int destination);
};

//on cpp

#if !_WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#endif

NetworkConnection::NetworkConnection(int port)
	: _socket(-1), _maxConnections(10)
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
	: _socket(-1), _maxConnections(1)
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

void NetworkConnection::setToNonBlocking() {
	unsigned long on = 1;
	int rc = -1;
#if _WIN32

	rc = setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
	//windows stuff to make socket non blocking
	if (rc >= 0)
		rc = ioctlsocket(_socket, FIONBIO, &on);

#else
	//make socket non blocking
	setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
	rc = ioctl(_socket, FIONBIO, (char *)&on);
#endif
	if (rc < 0) {
		/*error making non blocking socket*/
		close(_socket);
	}
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

inline bool NetworkConnection::sendTo(const char * data, int len, int socket)
{
	rc = send(socket, data, len, 0);//eq to write
	if (rc < 0)
	{
		perror("send() failed");
		close_conn = TRUE;
		return false;
	}
	return true;
}

int checkSelect(int max_sd, fd_set& working_set, timeval& timeout) {
	/**********************************************************/
	/* Call select() and wait for it to complete.   */
	/**********************************************************/
	printf("Waiting on select()...\n");
	int rc = select(max_sd + 1, &working_set, NULL, NULL, &timeout);

	/**********************************************************/
	/* Check to see if the select call failed.                */
	/**********************************************************/
	if (rc < 0)
	{
		perror("  select() failed");
		return -1;
	}

	/**********************************************************/
	/* Check to see if the time out expired.         */
	/**********************************************************/
	if (rc == 0)
	{
		printf("  select() timed out.  End program.\n");
		return -1;
	}
	return rc;
}

template <typename T>
inline std::map<int, T*> NetworkConnection::readAsync()
{
	if (!std::is_base_of<ISerializable, T>::value)
	{
		throw std::exception("T base type must be of type ISerializable");
	}

	std::map<int, T*> dataOfSocket;

	int desc_ready;
	/**********************************************************/
	/* Copy the master fd_set over to the working fd_set.     */
	/**********************************************************/
	memcpy(&working_set, &master_set, sizeof(master_set));

	rc = checkSelect(max_sd, working_set, timeout);
	if (rc < 1) return dataOfSocket;
	/**********************************************************/
	/* One or more descriptors are readable.  Need to         */
	/* determine which ones they are.                         */
	/**********************************************************/
	desc_ready = rc;
	for (i = 0; i <= max_sd && desc_ready > 0; ++i)
	{
		/*******************************************************/
		/* Check to see if this descriptor is ready            */
		/*******************************************************/
		if (FD_ISSET(i, &working_set))
		{
			/****************************************************/
			/* A descriptor was found that was readable - one   */
			/* less has to be looked for.  This is being done   */
			/* so that we can stop looking at the working set   */
			/* once we have found all of the descriptors that   */
			/* were ready.                                      */
			/****************************************************/
			desc_ready -= 1;

			/****************************************************/
			/* Check to see if this is the listening socket     */
			/****************************************************/
			if (i == _socket)
			{
				printf("  Listening socket is readable\n");
				/*************************************************/
				/* Accept all incoming connections that are      */
				/* queued up on the listening socket before we   */
				/* loop back and call select again.              */
				/*************************************************/
				do
				{
					/**********************************************/
					/* Accept each incoming connection.  If       */
					/* accept fails with EWOULDBLOCK, then we     */
					/* have accepted all of them.  Any other      */
					/* failure on accept will cause us to end the */
					/* server.                                    */
					/**********************************************/
					new_sd = accept(_socket, NULL, NULL);
					if (new_sd < 0)
					{
						if (errno != EWOULDBLOCK)
						{
							perror("  accept() failed");
							return dataOfSocket;
						}
						break;
					}

					/**********************************************/
					/* Add the new incoming connection to the     */
					/* master read set                            */
					/**********************************************/
					printf("  New incoming connection - %d\n", new_sd);
					//read
					FD_SET(new_sd, &master_set);
					if (new_sd > max_sd)
						max_sd = new_sd;

					/**********************************************/
					/* Loop back up and accept another incoming   */
					/* connection                                 */
					/**********************************************/
				} while (new_sd != -1);
			}

			/****************************************************/
			/* This is not the listening socket, therefore an   */
			/* existing connection must be readable             */
			/****************************************************/
			else
			{
				printf("  Descriptor %d is readable\n", i);
				/*************************************************/
				/* Receive all incoming data on this socket      */
				/* before we loop back and call select again.    */
				/*************************************************/
				do
				{
					/**********************************************/
					/* Receive data on this connection until the  */
					/* recv fails with EWOULDBLOCK.  If any other */
					/* failure occurs, we will close the          */
					/* connection.                                */
					/**********************************************/
					char buffer[MAX_BUFFER_SIZE];
					rc = recv(i, buffer, MAX_BUFFER_SIZE, 0);
					if (rc < 0)
					{
						if (errno != EWOULDBLOCK)
						{
							perror("  recv() failed");
							close_conn = TRUE;
						}
						break;
					}

					/**********************************************/
					/* Check to see if the connection has been    */
					/* closed by the client                       */
					/**********************************************/
					if (rc == 0)
					{
						printf("  Connection closed\n");
						close_conn = TRUE;
						break;
					}

					/**********************************************/
					/* Data was received                          */
					/**********************************************/
					len = rc;
					printf("  %d bytes received\n", len);
					dataOfSocket[i] = new T();
					dataOfSocket[i]->deserialize(buffer);
				} while (TRUE);

				/*************************************************/
				/* If the close_conn flag was turned on, we need */
				/* to clean up this active connection.  This     */
				/* clean up process includes removing the        */
				/* descriptor from the master set and            */
				/* determining the new maximum descriptor value  */
				/* based on the bits that are still turned on in */
				/* the master set.                               */
				/*************************************************/
				//if (close_conn)
				//{
				//	close(i);
				//	FD_CLR(i, &master_set);
				//	if (i == max_sd)
				//	{
				//		while (FD_ISSET(max_sd, &master_set) == FALSE)
				//			max_sd -= 1;
				//	}
				//}
			} /* End of existing connection is readable */
		} /* End of if (FD_ISSET(i, &working_set)) */
	} /* End of loop through selectable descriptors */

	return dataOfSocket;
}

inline bool NetworkConnection::sendData(const char * data, int len)
{
	return sendTo(data, len, _socket);
}
