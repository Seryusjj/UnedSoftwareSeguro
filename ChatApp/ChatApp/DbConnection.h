#pragma once
#include <iostream>
#include "sqlite3.h"
#include <vector>
#include <map>
#include <exception>

class DbConnection
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

public:
	DbConnection(const std::string name) 
	{
		dbName = name;
	}

	~DbConnection() 
	{

	}
	void connect() 
	{
		rc = sqlite3_open(dbName.c_str(), &db);
		throwSQLiteException(rc);
	}

	void end() 
	{
		sqlite3_close(db);
	}

	std::map<std::string, std::vector<std::string>>
	executeQuery(std::string query) 
	{
		std::map<std::string, std::vector<std::string>> resultset;
		char ** results = NULL;
		int rows, columns;
		rc = sqlite3_get_table(db, query.c_str(), &results, &rows, &columns, &error);
		throwSQLiteException(rc);

		for (int i = 0; i < columns; i++)
		{
			std::vector<std::string> rowResults;
			for (int j = 0; j < rows ; j++)
			{
				int cellPosition = (j * columns) + i;
				if (cellPosition >= columns) 
				{
					rowResults.push_back(results[cellPosition]);
				}
				
			}
			resultset[results[i]] = rowResults;
		}
		sqlite3_free_table(results);
		return resultset;
	}

	void createInsertOrUpdate(std::string query)
	{
		rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &error);
		throwSQLiteException(rc);
	}

	void printTable(std::map<std::string, std::vector<std::string>> resultSet)
	{
		for (auto& rows : resultSet)
		{
			std::cout << rows.first.c_str() << ": ";
			for (unsigned int j = 0; j < rows.second.size(); j++)
			{
				std::cout << rows.second[j].c_str() << " ";
			}
			std::cout << std::endl;
		}
	}




};

