#pragma once

#include "ClientConnection.h"
#include "ServerConnection.h"

static class EXPORT NetworkingFactory
{
public:
	static ClientConnection* CreateClientConnection(int serverPort, std::string serverIp)
	{
		return new ClientConnection(serverPort, serverIp);
	}

	static ServerConnection* CreateServerConnection(int listeningPort)
	{
		return new ServerConnection(listeningPort);
	}
};
