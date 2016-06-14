#include <iostream>;
#include "User.h"
#include <thread>
#include <future>
#include <chrono>
#include "NetworkingFactory.h"
#include "OperationCodes.h"
#include <sstream>

User* userdata;

std::mutex mutex;
typedef std::lock_guard<std::mutex> lock;
std::map<int, User*> totalUsers;//users organized by id

bool logInORCreate(ClientConnection* connection)
{
	connection->sendData(userdata->serialize(), userdata->serializeSize());
	User * res = connection->readBlocking<User>();
	switch (res->Id)
	{
	case LOGIN_ERROR:
		return false;
	case CREATEUSER_ERROR:
		return false;
	default:
		delete userdata;
		userdata = res;
		return true;
	}
}

int menuLogin(ClientConnection* connection)
{
	std::cout << "Select an option, 0 to close client" << std::endl;
	std::cout << "select 1 for log in " << std::endl;//also creates user if does not exists
	std::cout << "select 2 for create user " << std::endl;//also creates user if does not exists

	int selected = -1;
	char option[5];
	char username[10];
	char pass[20];
	char pass1[20];
	std::cin.getline(option, 5, '\n');
	std::stringstream ss(option);
	ss >> selected;
	if (select > 0)
	{
		switch (selected)
		{
		case 1:
			std::cout << "Write a username no longer than 10 characters" << std::endl;
			std::cin.getline(username, 10, '\n');
			std::cout << "Write a password no longer than 20 characters" << std::endl;
			std::cin.getline(pass, 20, '\n');
			//some checks
			//end checks
			userdata->setUsername(username);
			userdata->setPassword(pass);
			userdata->Id = LOGIN;
			if (logInORCreate(connection)) return 0;//done
			std::cout << "User or pass are incorrect, try again" << std::endl;
			return 1;
		case 2:
			std::cout << "Write a username no longer than 10 characters" << std::endl;
			std::cin.getline(username, 10, '\n');
			std::cout << "Write a password no longer than 20 characters" << std::endl;
			std::cin.getline(pass, 20, '\n');
			std::cout << "Write the password again" << std::endl;
			std::cin.getline(pass1, 20, '\n');

			//some checks
			userdata->Id = CREATEUSER;
			userdata->setUsername(username);
			userdata->setPassword(pass);
			if (logInORCreate(connection)) return 0;//done
			std::cout << "The username already exists, choose another one" << std::endl;
			return 1;
		default:
			return -1;
		}
	}
	return -1;
}

void managedCode(ClientConnection* connection) {
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	lock l(mutex);
	//send my data
	connection->sendData(userdata->serialize(), userdata->serializeSize());
	int numUsers = connection->readBlockingIntData();
	while (numUsers > 0)
	{
		User * res = connection->readBlocking<User>();
		if (totalUsers[res->Id] != NULL) {
			delete totalUsers[res->Id];
		}
		totalUsers[res->Id] = res;

		connection->sendIntData(-15);
		numUsers--;
	}
}

void sendData(ClientConnection* connection)
{
	while (connection->getSocketNumber() != -1)
	{
		try {
			//sleep 1s
			managedCode(connection);
		}
		catch (std::exception e)
		{
			//totalUsers.clear();
		}
	}
}

int menuGame()
{
	std::cout << "Select an option, 0 to close client" << std::endl;
	std::cout << "1 to change position, you got 10 digits per coordinate" << std::endl;
	std::cout << "2 to visualize other players position" << std::endl;
	char option[5];
	int selected = -1;
	char posx[10]{ 0 };
	char posy[10]{ 0 };
	char posz[10]{ 0 };
	std::cin.getline(option, 5, '\n');
	std::stringstream ss(option);
	ss >> selected;
	if (select != 0)
	{
		lock l(mutex);
		switch (selected)
		{
		case 1:

			std::cout << "Inser X coordinate" << std::endl;
			std::cin.getline(posx, 10, '\n');
			ss = std::stringstream(posx);
			ss >> userdata->X;
			std::cout << "Inser Y coordinate" << std::endl;
			std::cin.getline(posy, 10, '\n');
			ss = std::stringstream(posy);
			ss >> userdata->Y;
			std::cout << "Inser Z coordinate" << std::endl;
			std::cin.getline(posy, 10, '\n');
			ss = std::stringstream(posy);
			ss >> userdata->Z;

			return 1;
		case 2:
			for each (auto pair in totalUsers)
			{
				std::cout << "User: " << pair.second->getUserName() << " PosX: " << pair.second->X << " PosY: "
					<< pair.second->Y << " PosZ: " << pair.second->Z << std::endl;
			}
			return 2;
		default:
			std::cout << "Select a valid option" << std::endl;
			return -1;
		}
	}
}

int main()
{
	ClientConnection* connection = NetworkingFactory::CreateClientConnection(4000, "127.0.0.1");
	userdata = new User();
	connection->open();

	int login = -1;

	while (login != 0) { login = menuLogin(connection); }

	if (login != -1) {
		std::thread t(sendData, connection);
		while (menuGame() != 0) { menuGame(); }
	}
	//lock l(mutex);
	connection->close();
	delete connection;
	return EXIT_SUCCESS;
}