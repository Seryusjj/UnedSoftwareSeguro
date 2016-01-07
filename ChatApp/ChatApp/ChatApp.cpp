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
		connection.createInsertOrUpdate("CREATE TABLE MyTable (id INTEGER PRIMARY KEY, value STRING);");
	}
	catch (exception &e)
	{
		cout << e.what() << endl;
	}
	connection.end();
}

void insertData()
{
	try
	{
		connection.connect();
		connection.createInsertOrUpdate("INSERT INTO MyTable VALUES(NULL, 'A Value');");
	}
	catch (exception &e)
	{
		cout << e.what() << endl;
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
		auto resultSet = connection.executeQuery("SELECT * FROM MyTable;");
		connection.printTable(resultSet);
	}
	catch (exception &e)
	{
		cout << e.what() << endl;
	}
	connection.end();
	cin.get();
	return 0;
}



