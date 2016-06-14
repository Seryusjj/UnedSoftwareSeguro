#include "User.h"
#include "SerializablePOD.h"
#include <string>

User::User()
{
	Id = -1;
	X = Y = Z = 0.0f;
	_message = new char[1]{ "" };
	_username = new char[1]{ "" };
	_password = new char[1]{ "" };
}

User::~User()
{
	delete[] _message;
	delete[] _username;
	delete[] _password;
}

int32_t User::serializeSize() const
{
	int32_t totalSize = SerializablePOD<float>::serializeSize(X);
	totalSize += SerializablePOD<float>::serializeSize(Y);
	totalSize += SerializablePOD<float>::serializeSize(Z);
	totalSize += SerializablePOD<char*>::serializeSize(_message);
	totalSize += SerializablePOD<int64_t>::serializeSize(Id);
	totalSize += SerializablePOD<char*>::serializeSize(_username);
	totalSize += SerializablePOD<char*>::serializeSize(_password);
	return totalSize;
}

char* User::serialize()
{
	int32_t dataLength = serializeSize();
	char * dataBuffer = new char[dataLength];
	dataBuffer = SerializablePOD<float>::serialize(dataBuffer, X);
	dataBuffer = SerializablePOD<float>::serialize(dataBuffer, Y);
	dataBuffer = SerializablePOD<float>::serialize(dataBuffer, Z);
	//if the message is "" i'm loosing 4+1 bytes for dummy data
	//may be the messages should go in another channel
	dataBuffer = SerializablePOD<char*>::serialize(dataBuffer, _message);
	dataBuffer = SerializablePOD<int64_t>::serialize(dataBuffer, Id);
	dataBuffer = SerializablePOD<char*>::serialize(dataBuffer, _username);
	dataBuffer = SerializablePOD<char*>::serialize(dataBuffer, _password);
	return dataBuffer - dataLength;
}

void User::deserialize(const char* dataToDeserialize)
{
	dataToDeserialize = SerializablePOD<float>::deserialize(dataToDeserialize, X);
	dataToDeserialize = SerializablePOD<float>::deserialize(dataToDeserialize, Y);
	dataToDeserialize = SerializablePOD<float>::deserialize(dataToDeserialize, Z);
	dataToDeserialize = SerializablePOD<char*>::deserialize(dataToDeserialize, _message);
	dataToDeserialize = SerializablePOD<int64_t>::deserialize(dataToDeserialize, Id);
	dataToDeserialize = SerializablePOD<char*>::deserialize(dataToDeserialize, _username);
	dataToDeserialize = SerializablePOD<char*>::deserialize(dataToDeserialize, _password);
}

void inline User::setMessage(char * newMessage)
{
	if (newMessage == NULL)
	{
		delete[] _message;
		_message = new char[1]{ "" };
	}
	else
	{
		int size = strlen(newMessage) + 1;
		if (size > 140)
		{
			throw new std::exception("Mesage is longer than 140 characters");
		}
		delete[] _message;
		_message = new char[size];
		strcpy(_message, newMessage);
	}
}

void inline User::setUsername(char * username)
{
	if (username == NULL)
	{
		delete[] _username;
		_username = new char[1]{ "" };
	}
	else
	{
		int size = strlen(username) + 1;
		if (size > 10)
		{
			throw new std::exception("username longer than 10 characters");
		}
		delete[] _username;
		_username = new char[size];
		strcpy(_username, username);
	}
}

void inline User::setPassword(char * newPassword)
{
	if (newPassword == NULL)
	{
		delete[] _password;
		_password = new char[1]{ "" };
	}
	else
	{
		int size = strlen(newPassword) + 1;
		if (size > 20)
		{
			throw new std::exception("Password longer than 20 characterers");
		}
		delete[] _password;
		_password = new char[size];
		strcpy(_password, newPassword);
	}
}