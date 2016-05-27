#include <iostream>
#include "User.h"
#include "NetworkingFactory.h"

using namespace std;

int main(void)
{
	ServerConnection* connection = NetworkingFactory::CreateServerConnection(4000);
	connection->open();
	while (true)
	{
		//leemos datos de todos los usuarios conectados
		auto result = connection->readAsync<User>();
		for each (auto pair in result)
		{
			//send the user the number of users connected
			connection->sendIntFrom(result.size(), pair.first);
		}
		/** If a client does not end precessing data, its not going to send anything that is, is not going to be on the result ***/
		/** If a user disconnect from server is not going to be on result map anymore but is going to be on clients list so we have to tell**/
		/** The client withc users are still connected **/
		//send to each user ther others user data in another thread
		for each (auto pair in result)
		{
			for each (auto pairTwo in result)
			{
				//do not send user a it's own data
				if (pair.first != pairTwo.first)
					connection->sendFrom(pairTwo.second->serialize(), pairTwo.second->serializeSize(), pair.first);
			}
		}
	}
	connection->close();
	return EXIT_SUCCESS;
}