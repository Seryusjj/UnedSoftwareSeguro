#pragma once
#include "NetworkConnection.h"

class EXPORT ClientConnection : public NetworkConnection
{
	friend class NetworkingFactory;
private:
	ClientConnection(int port, std::string ip);
protected:
	virtual bool bindSocket() override;
	virtual void postSocketCreation() override {};
	virtual void setToNonBlocking() override {};

public:

	~ClientConnection();
	sockaddr_in getClientInfo() { return _client; }
};
