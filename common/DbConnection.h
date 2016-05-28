#pragma once
#include "DLLDefines.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <exception>

class sqlite3;

class EXPORT DbConnection
{
private:
	sqlite3 *db;
	char* error;
	int rc;
	std::string dbName;

	void throwSQLiteException(int excep);
	DbConnection() {}
public:

	DbConnection(const std::string name)
	{
		dbName = name;
	}

	~DbConnection()
	{
	}

	void open();

	void close();

	std::map<std::string, std::vector<std::string>> executeQuery(std::string query);

	void createInsertOrUpdate(std::string query);

	void printTable(std::map<std::string, std::vector<std::string>> & const resultSet);
};
