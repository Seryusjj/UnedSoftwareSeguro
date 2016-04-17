#include <iostream>
#include "User.h"

int main()
{
	User user1;
	user1.posX = 3;
	user1.posY = 3;
	user1.posZ = 3;
	char*  data = user1.serialize();
	char*  data1 = user1.serialize();
	user1.deserialize(data);
	delete data;
	user1.deserialize(data1);
	delete data1;
	std::cout << "Client started" << " User-> x: " << user1.posX << " y: " << user1.posY << " z: " << user1.posZ << std::endl;
}