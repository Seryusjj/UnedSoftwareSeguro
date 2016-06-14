#pragma once

#include "UserGateway.h"
#include <sstream>

#define UsernameIndex 1
#define PassIndex 2
#define PosXIndex 3
#define PosYIndex 4
#define PosZIndex 5
#define IdIndex 6

int64_t UserGateway::generateNextId()
{
	char * sql = "SELECT Id FROM users;";
	sqlite3_stmt* stat = connection.createPreparedStatement(sql);
	int64_t id = 0;
	int64_t maxId = -1;
	int rc;
	do {
		rc = sqlite3_step(stat);
		id = sqlite3_column_int64(stat, 0);
		if (id > maxId) maxId = id;
	} while (rc == SQLITE_ROW);
	return ++maxId;
}

UserGateway::UserGateway(DbConnection& connection) : connection(connection)
{
}

UserGateway::~UserGateway()
{
}

void UserGateway::CreateUsersTable() {
	try {
		connection.createInsertOrUpdate("CREATE TABLE users (username STRING PRIMARY KEY, pass STRING, posX REAL, posY REAL, posZ REAL, Id INTEGER UNIQUE);");
	}
	catch (std::exception e) {
	}
}

void UserGateway::createNewUser(User* user)
{
	std::string sql = "INSERT INTO users (username, pass, posX, posY, posZ, Id) VALUES (?, ?, ?, ?, ?, ?);";
	sqlite3_stmt* stat = connection.createPreparedStatement(sql);
	int64_t nextId = generateNextId();
	user->Id = nextId;

	sqlite3_bind_text(stat, UsernameIndex, user->getUserName().c_str(), user->getUserName().size(), SQLITE_TRANSIENT);
	sqlite3_bind_text(stat, PassIndex, user->getPass().c_str(), user->getPass().size(), SQLITE_TRANSIENT);
	sqlite3_bind_double(stat, PosXIndex, user->X);
	sqlite3_bind_double(stat, PosYIndex, user->Y);
	sqlite3_bind_double(stat, PosZIndex, user->Z);
	sqlite3_bind_int64(stat, IdIndex, user->Id);
	int rc = sqlite3_step(stat);
	if (rc != SQLITE_DONE)
		throw std::exception("Cannot create user");
}

void UserGateway::updateUser(User * data)
{
}

User* UserGateway::getUserById(int64_t Id) {
	User * result = new User();
	std::string sql = "SELECT username, pass, posX, posY, posZ, Id FROM users WHERE Id = ?;";
	sqlite3_stmt* stat = connection.createPreparedStatement(sql);
	sqlite3_bind_int64(stat, IdIndex, Id);
	int rc = -1;
	do {
		rc = sqlite3_step(stat);
		int val = sqlite3_column_int64(stat, 5);
	} while (rc == SQLITE_ROW);

	sqlite3_finalize(stat);
	return nullptr;
}

User* UserGateway::getUserByUserAndPass(std::string username, std::string pass) {
	User* usr = getUserByUserName(username);
	if (usr == nullptr) return nullptr;
	if (usr->getPass() == pass) return usr;
	return nullptr;
}

void  buildUser(sqlite3_stmt* stat, User * result)
{
	char* username = (char*)sqlite3_column_text(stat, UsernameIndex - 1);
	char* pass = (char*)sqlite3_column_text(stat, PassIndex - 1);

	result->setUsername(username);
	result->setPassword(pass);
	result->X = sqlite3_column_double(stat, PosXIndex - 1);
	result->Y = sqlite3_column_double(stat, PosYIndex - 1);
	result->Z = sqlite3_column_double(stat, PosZIndex - 1);
	result->Id = sqlite3_column_int64(stat, IdIndex - 1);
}

User* UserGateway::getUserByUserName(std::string username)
{
	try {
		User* result = new User();
		std::string sql = "SELECT username, pass, posX, posY, posZ, Id FROM users WHERE username = ?;";
		sqlite3_stmt* stat = connection.createPreparedStatement(sql);
		sqlite3_bind_text(stat, UsernameIndex, username.c_str(), username.size(), SQLITE_TRANSIENT);

		int rc = -1;
		do {
			rc = sqlite3_step(stat);
			char* val = (char*)sqlite3_column_text(stat, UsernameIndex);

			if (val == NULL) {
				return nullptr;
			}
			if (username == std::string(val)) {
				buildUser(stat, result);
				break;
			}
		} while (rc == SQLITE_ROW);

		sqlite3_finalize(stat);
		return result;
	}
	catch (std::exception e)
	{
		return nullptr;
	}
}