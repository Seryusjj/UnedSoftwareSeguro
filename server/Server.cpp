#include <iostream>
#include "User.h"
#include "NetworkingFactory.h"
#include "ConfigurationManager.h"
#include "DbConnection.h"

using namespace std;
std::map<int, User*> connected;

void createDb(DbConnection& connection)
{
	connection.open();
	connection.createInsertOrUpdate("CREATE TABLE users (username STRING PRIMARY KEY, pass STRING);");
	connection.close();
}

int main(void)
{
	ConfigurationManager& manager = ConfigurationManager::getInstance();
	manager.loadDataOfFile("server.properties");

	int port = manager.getValueOfPropertie<int>("Server.Port");
	std::string dbName = manager.getValueOfPropertie<std::string>("Server.DbName");

	ServerConnection* socketConnection = NetworkingFactory::CreateServerConnection(4000);
	DbConnection dbConnection = DbConnection(dbName);
	socketConnection->open();
	dbConnection.open();
	while (true)
	{
		//leemos datos de todos los usuarios conectados
		auto result = socketConnection->readAsync<User>();
		for each (auto pair in result)
		{
			connected[pair.first] = pair.second;
		}

		//verificamos que estan logueados #thread 1
		//id!=-1 si no estan logueados los mandamos a login si no continuamos

		//onece the user is logged in, just send it's data to to other players and vice versa #thread 2
		for each (auto pair in connected)
		{
			//send the user the number of users connected != from current
			socketConnection->sendIntFrom(result.size() - 1, pair.first);
		}
		/** If a client does not end precessing data, its not going to send anything that is, is not going to be on the result ***/
		/** If a user disconnect from server is not going to be on result map anymore but is going to be on clients list so we have to tell**/
		/** The client withc users are still connected **/
		//send to each user ther others user data in another thread
		for each (auto pair in connected)
		{
			for each (auto pairTwo in connected)
			{
				//do not send user a it's own data
				if (pair.first != pairTwo.first)
					socketConnection->sendFrom(pairTwo.second->serialize(), pairTwo.second->serializeSize(), pair.first);
			}
		}
	}
	socketConnection->close();
	dbConnection.close();
	return EXIT_SUCCESS;
}