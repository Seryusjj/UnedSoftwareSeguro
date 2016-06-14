#pragma once
#include "DLLDefines.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <exception>
#include "sqlite3.h"

class EXPORT DbConnection
{
private:
	sqlite3 *db;
	char* error;
	int rc;
	std::string dbName;

	void throwSQLiteException(int excep)
	{
		if (excep)
		{
			std::exception excepcionToThrow = std::exception(sqlite3_errmsg(db));
			sqlite3_free(error);
			throw  excepcionToThrow;
		}
	}
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

	sqlite3_stmt* createPreparedStatement(std::string query) {
		sqlite3_stmt *res;
		int  rc = sqlite3_prepare_v2(db, query.c_str(), -1, &res, 0);
		throwSQLiteException(rc);
		return res;
	}

	void createInsertOrUpdate(std::string query);

	void printTable(std::map<std::string, std::vector<std::string>> & const resultSet);
};
