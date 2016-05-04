#pragma once

#include "NetworkConnection.h"

class ClientConnection : public NetworkConnection
{
protected:
	virtual bool bindSocket() override;
	virtual void postSocketCreation() override {};
	virtual void setToNonBlocking() override {};
public:

	ClientConnection(int port, std::string ip);
	~ClientConnection();
	sockaddr_in getClientInfo() { return _client; }
	void setServerInfo(sockaddr_in& server) { _server = server; }
private:
};

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
