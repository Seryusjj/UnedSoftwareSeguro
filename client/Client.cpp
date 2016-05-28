#include <iostream>;
#include "User.h"
#include <thread>
#include <future>
#include <chrono>
#include "NetworkingFactory.h"

std::mutex mutex;
typedef std::lock_guard<std::mutex> lock;

void sendData(ClientConnection* connection, User* data)
{
	int numUsers = 0;
	while (connection->getSocketNumber() != -1)
	{
		//sleep 1s
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		//lock
		//lock l(mutex);//if conn is close we sill wait til client is closed
		connection->sendData(data->serialize(), data->serializeSize());
		numUsers = connection->readBlockingIntData();
		while (numUsers != 0)
		{
			connection->readBlocking<User>();
			numUsers--;
		}
		//release lock
	//	l.~l();
	}
}

int menu()
{
	std::cout << "Select an option, 0 to close client" << std::endl;
	std::cout << "1 to change position" << std::endl;
	std::cout << "2 to write a message" << std::endl;
	std::cout << "3 to visualize other players position and messages" << std::endl;
	int selected = -1;
	std::cin >> selected;
	if (select != 0)
	{
		switch (selected)
		{
		case 1:
			return 1;
		case 2:
			return 2;
		case 3:
			return 3;
		default:
			std::cout << "Select a valid option" << std::endl;
			return -1;
		}
	}
}

int main()
{
	ClientConnection* connection = NetworkingFactory::CreateClientConnection(4000, "127.0.0.1");
	User* userdata = new User();
	connection->open();
	std::thread t(sendData, connection, userdata);

	while (menu() != 0) { menu(); }
	//lock l(mutex);
	connection->close();
	delete connection;
	return EXIT_SUCCESS;
}