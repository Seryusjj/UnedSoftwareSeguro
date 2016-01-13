// ChatApp.cpp : Defines the entry point for the console application.
//


#include "DbConnection.h"

using namespace std;
DbConnection connection("MyDb.db");

void createTables()
{
	try
	{
		connection.connect();
		connection.createInsertOrUpdate("CREATE TABLE users (username STRING PRIMARY KEY, pass STRING);");
	}
	catch (exception &e)
	{
		//cout << e.what() << endl;
	}
	connection.end();
}

void insertData()
{
	try
	{
		connection.connect();
		connection.createInsertOrUpdate("INSERT INTO users VALUES('sergio1', 'sergio1234');");
		connection.createInsertOrUpdate("INSERT INTO users VALUES('maria', 'maria1234');");
		connection.createInsertOrUpdate("INSERT INTO users VALUES('manolo', 'manuel1234');");
		connection.createInsertOrUpdate("INSERT INTO users VALUES('sergio', 'sergio1234');");
	}
	catch (exception &e)
	{
		//cout << e.what() << endl;
	}
	connection.end();
}



int main()
{
	createTables();
	insertData();
	try
	{
		connection.connect();
		char pass[15];
		char username[15];

		std::cout << "Enter your username: ";
		std::cin >> username;
		std::cout << "Enter your password: ";
		std::cin >> pass;


		std::string query = "SELECT * FROM users WHERE pass='" + std::string(pass) + "' and username = '"+std::string(username)+"';";
		
		auto resultSet = connection.executeQuery(query);
		connection.printTable(resultSet);
	}
	catch (exception &e)
	{
		cout << e.what() << endl;
	}
	connection.end();
	std::cout << "Press a key to exit program" << std::endl;
	std::cin.get();
	std::cin.get();
	return 0;
}



