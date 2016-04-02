#include <iostream>
#include "User.h"

int main()
{
	User user;
	user.posX = 3;
	user.posY = 3;
	user.posZ = 3;

	std::cout << "Client started" << " User-> x: " << user.posX << " y: " << user.posY << " z: " << user.posZ << std::endl;
}