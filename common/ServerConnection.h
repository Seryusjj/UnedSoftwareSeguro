#pragma once

#include "NetworkConnection.h"

class ServerConnection :public NetworkConnection
{
protected:
	virtual bool bindSocket() override;
	virtual void postSocketCreation() override;
public:
	ServerConnection(int port);
	~ServerConnection();
	sockaddr_in getServerInfo() { return _server; }
};

inline bool ServerConnection::bindSocket()
{
	/* Bind local address to socket */
	if (bind(_socket, (struct sockaddr *)&_server, sizeof(struct sockaddr_in)) < 0)
	{
		//"Cannot bind address to socket.");
		close(_socket);
		return false;
	}

	return true;
}

ServerConnection::ServerConnection(int port)
	:NetworkConnection(port)
{
}

ServerConnection::~ServerConnection()
{
}

inline void ServerConnection::postSocketCreation()
{
	/* listen on the specified port a max of connections*/
	/* call  applies only to sockets of type SOCK_STREAM or SOCK_SEQPACKET */
	if (listen(_socket, _maxConnections) < 0)
	{
		/*Error on listen*/
		close(_socket);
	}
	/*************************************************************/
	/* Initialize the master fd_set                              */
	/*************************************************************/
	FD_ZERO(&master_set);
	max_sd = _socket;
	// add the listener to the master set
	FD_SET(_socket, &master_set);

	/*************************************************************/
	/* Initialize the timeval struct to 3 minutes.  If no        */
	/* activity after 3 minutes this program will end.           */
	/*************************************************************/
	timeout.tv_sec = 3 * 60;
	timeout.tv_usec = 0;
}
