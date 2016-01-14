// ChatApp.cpp : Defines the entry point for the console application.
//


#include "DbConnection.h"
#include <windows.h>

using namespace std;
DbConnection connection("MyDb.db");


struct UserData
{
	char username[20];
	char pass[20];
};

UserData * userdataPointer;

void debugInfoLog(char * message) 
{
	std::cout << message << std::endl;
}

void initUserDataPointer(const char* user, const  char * pass)
{
	UserData userdata;
	strcpy(userdata.username, user);
	strcpy(userdata.pass, pass);
	userdataPointer = &userdata;
	new(userdataPointer) UserData;
}

void freeUserDataPointer()
{
	//silenciar el error
	try 
	{
		free(userdataPointer);
		free(userdataPointer);
	}
	catch (exception &e) {
	}
	
}



void createTables()
{
	connection.connect();
	debugInfoLog("Creando tablas de base de datos");
	try
	{
		connection.createInsertOrUpdate("CREATE TABLE users (username STRING PRIMARY KEY, pass STRING);");
	}
	catch (exception &e)
	{
		//cout << e.what() << endl;
	}
	connection.end();
}



void insertUser(char * username, char * pass)
{
	try
	{
		connection.connect();
		connection.createInsertOrUpdate("INSERT INTO users VALUES('" + std::string(username) + "', '" + std::string(pass) + "');");
	}
	catch (exception &e)
	{
		//cout << e.what() << endl;
	}
	connection.end();
}

void insertData()
{
	debugInfoLog("Creando usuarios por defecto para la base de datos");
	insertUser("maria", "maria");
	insertUser("manolo", "manolo");
	insertUser("sergio", "sergio");
	insertUser("Juanjo", "");
}



bool loginAsAdmin(char * user, char* pass)
{
	debugInfoLog("Intenando login como admin");
	char * admin = "admin";
	//harcoded pass
	char * adminPass = "admin";

	return !strcmp(user, admin) && !strcmp(pass, adminPass);
}

void printUserInfo(const char * user, const char* pass)
{
	std::string query = "SELECT * FROM users WHERE pass='" + std::string(pass) + "' and username = '" + std::string(user) + "';";
	auto resultSet = connection.executeQuery(query);
	connection.printTable(resultSet);
}

void printAllUsers()
{
	std::string query = "SELECT * FROM users;";
	auto resultSet = connection.executeQuery(query);
	connection.printTable(resultSet);
}

void readUserAndPass(char * username, char * pass)
{
	std::cout << "Enter your username: ";
	std::cin >> username;
	std::cout << "Enter your password: ";
	std::cin >> pass;
}

int selectOptionMenuForNormalUser(const char * user, const char* pass);
int selectOptionMenuForAdmin(char * user, char* pass);
int main()
{
	createTables();
	insertData();
	try
	{
		char pass[15];
		char username[15];
		readUserAndPass(username, pass);

		if (loginAsAdmin(username, pass))
		{
			while (selectOptionMenuForAdmin(username, pass) != -1);
		}
		else
		{
			connection.connect();
			debugInfoLog("Intentando login como usuario normal");
			std::string query = "SELECT * FROM users WHERE pass='" + std::string(pass) + "' and username = '" + std::string(username) + "';";
			auto resultSet = connection.executeQuery(query);
			connection.end();

			if (resultSet.size() > 1)
			{
				while (selectOptionMenuForNormalUser(username, pass) != -1);
			}
		}


	}
	catch (exception &e)
	{

	}
	
	//doubel free
	freeUserDataPointer();
	std::cout << "Press a key to exit program" << std::endl;
	std::cin.get();
	std::cin.get();
	return 0;
}


int selectOptionMenuForNormalUser(const char * user, const char* pass)
{
	connection.connect();
	int option = -1;
	char input[10];
	initUserDataPointer(user, pass);
	userdataPointer = NULL;
	//memory leak
	initUserDataPointer(user, pass);

	std::cout << "Select one of the following options by tiping it's number " << std::endl;
	std::cout << "(Type other number not in the list to exit the program)" << std::endl;
	std::cout << "1 - List my user and pass" << std::endl;

	std::cin >> input;
	option = atoi(input);
	switch (option)
	{
	case 1:
		printUserInfo(user, pass);
		break;
	default:
		option = -1;
		break;
	}
	connection.end();
	return option;
}

int selectOptionMenuForAdmin(char * user, char* pass)
{
	connection.connect();
	initUserDataPointer(user, pass);
	int option = -1;
	char input[10];
	std::cout << "Select one of the following options by tiping it's number " << std::endl;
	std::cout << "(Type other number not in the list to exit the program)" << std::endl;
	std::cout << "1 - Integer overflow" << std::endl;
	std::cout << "2 - Create user" << std::endl;
	std::cout << "3 - List all users" << std::endl;
	std::cin >> input;
	option = atoi(input);
	switch (option)
	{
	case 1:
		while (option > 0)
		{
			option += 50 * option;
		}
		std::cout << "Integer overflow" << option << std::endl;
		break;
	case 2:
		char pass[15];
		char username[15];
		readUserAndPass(username, pass);
		insertUser(username, pass);
		break;
	case 3:
		printAllUsers();
		break;
	default:
		option = -1;
		break;
	}
	connection.end();
	return option;
}



