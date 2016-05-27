#include "User.h"
#include "SerializablePOD.h"
#include <string>

User::User()
{
	X = Y = Z = 0.0f;
	_message = new char[1]{ "" };
}

User::~User()
{
	delete[] _message;
}

int32_t User::serializeSize() const
{
	int32_t totalSize = SerializablePOD<float>::serializeSize(X);
	totalSize += SerializablePOD<float>::serializeSize(Y);
	totalSize += SerializablePOD<float>::serializeSize(Z);
	totalSize += SerializablePOD<char*>::serializeSize(_message);
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
	return dataBuffer - dataLength;
}

void User::deserialize(const char* dataToDeserialize)
{
	dataToDeserialize = SerializablePOD<float>::deserialize(dataToDeserialize, X);
	dataToDeserialize = SerializablePOD<float>::deserialize(dataToDeserialize, Y);
	dataToDeserialize = SerializablePOD<float>::deserialize(dataToDeserialize, Z);
	dataToDeserialize = SerializablePOD<char*>::deserialize(dataToDeserialize, _message);
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