#pragma once

#include "User.h"
#include "DbConnection.h"
#include "DLLDefines.h"
#include <map>

class EXPORT UserGateway
{
private:
	int64_t generateNextId();
	DbConnection& connection;

public:
	UserGateway(DbConnection& connection);
	void CreateUsersTable();
	void createNewUser(User* user);
	void updateUser(User* data);
	User *  getUserByUserAndPass(std::string username, std::string pass);
	User *  getUserByUserName(std::string username);
	User * getUserById(int64_t Id);

	~UserGateway();

private:
};
