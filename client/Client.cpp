#include <iostream>;
#include "User.h"
#include <future>
#include "NetworkingFactory.h"

std::mutex mutex;
typedef std::lock_guard<std::mutex> lock;

void sendData(ClientConnection* connection, User* data)
{
	int numUsers = 0;
	while (connection->getSocketNumber() != -1)
	{
		//lock
		lock l(mutex);//if conn is close we sill wait til client is closed
		connection->sendData(data->serialize(), data->serializeSize());
		numUsers = connection->readBlockingIntData();
		while (numUsers != 0)
		{
			connection->readBlocking<User>();
			numUsers--;
		}
		//release lock
		l.~l();
	}
}

void menu()
{
	std::cout << "Select an option, 0 to close client" << std::endl;
	std::cout << "1 to change position" << std::endl;
	std::cout << "2 to write a message" << std::endl;
	std::cout << "3 to visualize other players position and messages" << std::endl;
	int selected = 0;
	if (select != 0)
	{
		switch (selected)
		{
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		default:
			std::cout << "Select a valid option" << std::endl;
			menu();
			return;
		}
	}
}

int main()
{
	ClientConnection* connection = NetworkingFactory::CreateClientConnection(4000, "127.0.0.1");
	User* userdata = new User();
	connection->open();
	std::async(sendData, connection, userdata);

	menu();
	lock l(mutex);
	connection->close();
	delete connection;
	return EXIT_SUCCESS;
}