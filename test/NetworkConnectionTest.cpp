#include "gtest/gtest.h"
#include "User.h"
#include "NetworkingFactory.h"

/*Shared res*/

ClientConnection* client;
ClientConnection* auxclient;
ServerConnection* server;
User* usr;
User* destinyUsr;

// The fixture for testing class UserTest.
class NetworkConnectionTest : public ::testing::Test {
protected:

	NetworkConnectionTest()
	{
	}

	virtual ~NetworkConnectionTest()
	{
	}

	virtual void SetUp()
	{
		/*new server with port to listne on*/
		server = NetworkingFactory::CreateServerConnection(4000);
		server->open();
		//new client with server info
		client = NetworkingFactory::CreateClientConnection(4000, "127.0.0.1");
		auxclient = NetworkingFactory::CreateClientConnection(4000, "127.0.0.1");

		client->open();

		usr = new User();
		destinyUsr = new User();
		usr->setMessage("Hello I'm getting serialized");
	}

	virtual void TearDown()
	{
		client->close();
		server->close();
		delete client;
		delete auxclient;
		delete server;
		delete usr;
		delete destinyUsr;
	}
};

TEST_F(NetworkConnectionTest, OpenCloseConnectionTest)
{
	server->open();
	EXPECT_TRUE(server->getSocketNumber() >= 0);
	client->open();
	EXPECT_TRUE(client->getSocketNumber() >= 0);
	server->close();
	EXPECT_TRUE(server->getSocketNumber() < 0);
	client->close();
	EXPECT_TRUE(client->getSocketNumber() < 0);
}

TEST_F(NetworkConnectionTest, SendFromClientTest)
{
	std::map<int, User*> data;
	client->sendData(usr->serialize(), usr->serializeSize());

	//if no data keep reading
	while (data.size() == 0)
	{
		data = server->readAsync<User>();
	}
	EXPECT_TRUE(data.size() == 1);
	data.clear();
	usr->X = 10;
	if (client->sendData(usr->serialize(), usr->serializeSize()))
	{
		//if no data keep reading
		while (data.size() == 0)
		{
			data = server->readAsync<User>();
		}
	}
	EXPECT_TRUE(data.size() == 1);
	for each (auto  values in data)
	{
		User * received = (User*)values.second;
		EXPECT_TRUE(strcmp(usr->getMessage(), received->getMessage()) == 0);
	}
}

TEST_F(NetworkConnectionTest, SendFromSeveralClientsTest)
{
	auxclient->open();
	std::map<int, User*> data;
	client->sendData(usr->serialize(), usr->serializeSize());
	auxclient->sendData(usr->serialize(), usr->serializeSize());

	//if no data keep reading
	while (data.size() == 0)
	{
		data = server->readAsync<User>();
	}
	auxclient->close();
	EXPECT_TRUE(data.size() == 2);
}

TEST_F(NetworkConnectionTest, SendFromServerTest)
{
	std::map<int, User*> data;
	client->sendData(usr->serialize(), usr->serializeSize());
	//if no data keep reading
	while (data.size() == 0)
	{
		data = server->readAsync<User>();
	}
	for each (auto values in data)
	{
		server->sendFrom(values.second->serialize(), values.second->serializeSize(), values.first);
	}
	data.clear();

	User* received = client->readBlocking<User>();

	EXPECT_TRUE(strcmp(usr->getMessage(), received->getMessage()) == 0);
}