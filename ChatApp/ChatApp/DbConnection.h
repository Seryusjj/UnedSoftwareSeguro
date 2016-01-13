#pragma once
#include <iostream>
#include <algorithm>
#include "sqlite3.h"
#include <vector>
#include <map>
#include <exception>
#include <ios>

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
			for (int j = 0; j <= rows ; j++)
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

	void printTable( std::map<std::string, std::vector<std::string>> & const resultSet )
	{
		char* placeholder = "‹‹‹‹‹‹‹‹‹‹‹‹‹";
		int placeholderLength = strlen(placeholder);
		std::vector<std::string> keys;
		int numCol = 0;
		//print headers
		for (auto& colums : resultSet)
		{
			keys.push_back(colums.first);
			numCol++;
			std::string header(colums.first);
			std::transform(header.begin(), header.end(), header.begin(), toupper);
			std::cout.width(placeholderLength);
			std::cout.setf(std::ios::left);
			
			std::cout << header.c_str() << " ";
		}
		std::cout << std::endl;
		int numRows = resultSet[keys[0]].size();
		//print placeholder
		for (int i = 0; i < numCol; i++) 
		{
			std::cout.width(placeholderLength);
			std::cout.setf(std::ios::left);
			std::cout << placeholder;
		}
		std::cout << std::endl;
		//print row data
		for (int i = 0; i < numRows; i++)
		{
			for (int j = 0; j < numCol;j++)
			{
				int internelLength = j < numCol - 1 ? placeholderLength - 1: placeholderLength;
				std::cout.width(internelLength);
				std::cout.setf(std::ios::left);
				std::cout << resultSet[keys[j]][i].c_str();
				if (j < numCol - 1) std::cout << "€ ";

			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}


};

