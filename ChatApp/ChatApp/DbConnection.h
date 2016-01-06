#pragma once
#include <iostream>
#include "sqlite3.h"
#include <vector>
#include <map>

class DbConnection
{
private:
	sqlite3 *db;
	char* error;
	int rc;
	std::string dbName;
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
		if (rc) 
		{
			std::cerr << "Error executing SQLite3 statement: " << sqlite3_errmsg(db) << std::endl << std::endl;
			sqlite3_free(error);
		}
	}

};

