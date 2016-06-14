#include <iostream>
#include "User.h"
#include "NetworkingFactory.h"
#include "ConfigurationManager.h"
#include "DbConnection.h"
#include "UserGateway.h"
#include "OperationCodes.h"
#include <chrono>
#include <thread>

using namespace std;
std::map<int, User*> connected;
std::map<int, bool> loggedIn;

void loginOrCreate(std::map<int, User*>& result, UserGateway& usersGateway, ServerConnection* socketConnection) {
	//login + create user
	for each (auto pair in result)
	{
		//log in
		if (pair.second->Id == LOGIN)
		{
			User* res = usersGateway.getUserByUserAndPass(pair.second->getUserName(), pair.second->getPass());
			if (res == nullptr)
			{
				pair.second->Id = LOGIN_ERROR;
				socketConnection->sendIntFrom(LOGIN_ERROR, pair.first);
				delete pair.second;
			}
			else
			{
				socketConnection->sendFrom(res->serialize(), res->serializeSize(), pair.first);
				delete result[pair.first];
				result[pair.first] = res;
			}
		}//create user
		else if (pair.second->Id == CREATEUSER)
		{
			User* res = usersGateway.getUserByUserName(pair.second->getUserName());
			if (res == nullptr)
			{
				usersGateway.createNewUser(pair.second);
				socketConnection->sendFrom(pair.second->serialize(), pair.second->serializeSize(), pair.first);
			}
			else
			{
				pair.second->Id = CREATEUSER_ERROR;
				socketConnection->sendFrom(pair.second->serialize(), pair.second->serializeSize(), pair.first);
				delete pair.second;
			}
		}
	}
}
void copyUser(User* dest, User* source) {
	dest->Id = source->Id;
	dest->X = source->X;
	dest->Y = source->Y;
	dest->Z = source->Z;
	dest->setMessage((char*)source->getMessage());
	dest->setPassword((char*)source->getPass().c_str());
	dest->setUsername((char*)source->getUserName().c_str());
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
	UserGateway usersGateway(dbConnection);
	usersGateway.CreateUsersTable();
	while (true)
	{
		//leemos datos de todos los usuarios conectados
		auto ready = socketConnection->readAsync<User>();

		loginOrCreate(ready, usersGateway, socketConnection);
		for each (auto pair in ready)
		{
			if (pair.second->Id >= 0) {
				if (connected[pair.second->Id] != NULL)
					copyUser(connected[pair.second->Id], pair.second);
				else {
					connected[pair.second->Id] = new User();
					copyUser(connected[pair.second->Id], pair.second);
				}
			}
		}
		//send data to everybody
		//onece the user is logged in, just send it's data to to other players and vice versa #thread 2
		int totalUsers = connected.size() - 1;//all the users - the one who is requesting the data
		for each (auto pair in ready)
		{
			//send the user the number of users connected != from current
			socketConnection->sendIntFrom(totalUsers, pair.first);
		}
		//make the thread wait 1s for the last sended item to be received on target client
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		//send client data to each other clients and vice versa
		for each (auto pair in ready)
		{
			for each (auto pairTwo in connected)
			{
				//do not send user a it's own data
				if (pair.second->Id != pairTwo.second->Id) {
					socketConnection->sendFrom(pairTwo.second->serialize(), pairTwo.second->serializeSize(), pair.first);
					while (socketConnection->readBlockingIntDataFrom(pair.first) != -15) {
						//wait for response
					}
				}
			}
		}
	}
	socketConnection->close();
	dbConnection.close();
	return EXIT_SUCCESS;
}