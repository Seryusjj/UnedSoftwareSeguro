#pragma once

#include "ClientConnection.h"
#include "NetworkingFactory.h"

/*In clients is bind + connect*/
inline bool ClientConnection::bindSocket()
{
	/* Bind local address to socket */
	if (bind(_socket, (struct sockaddr *)&_client, sizeof(struct sockaddr_in)) < 0)
	{
		//"Cannot bind address to socket.");
		close(_socket);
		return false;
	}

	/*Connect*/
	int rc = connect(_socket, (struct sockaddr *)& _server, _serverLength);
	if (rc < 0)
	{
		close(_socket);
		return false;
	}
	return true;
}

ClientConnection::ClientConnection(int port, std::string ip)
	:NetworkConnection(port, ip)
{
}

ClientConnection::~ClientConnection()
{
}