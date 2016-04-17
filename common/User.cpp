#include "User.h"
#include "SerializablePOD.h"

User::User()
{
	posX = 0;
	posY = 0;
	posZ = 0;
	message = "";
}

User::~User()
{
}

size_t User::serializeSize() const
{
	size_t totalSize = 0;
	totalSize += SerializablePOD<int>::serializeSize(posX);
	totalSize += SerializablePOD<int>::serializeSize(posY);
	totalSize += SerializablePOD<int>::serializeSize(posZ);

	totalSize += SerializablePOD<char*>::serializeSize(message);
	return totalSize;
}

char* User::serialize()
{
	size_t dataLength = serializeSize();

	char * dataBuffer = new char[dataLength];

	dataBuffer = SerializablePOD<int>::serialize(dataBuffer, posX);
	dataBuffer = SerializablePOD<int>::serialize(dataBuffer, posY);
	dataBuffer = SerializablePOD<int>::serialize(dataBuffer, posZ);
	dataBuffer = SerializablePOD<char*>::serialize(dataBuffer, message);
	return dataBuffer - dataLength;
}

void User::deserialize(const char* dataToDeserialize)
{
	dataToDeserialize = SerializablePOD<int>::deserialize(dataToDeserialize, posX);
	dataToDeserialize = SerializablePOD<int>::deserialize(dataToDeserialize, posY);
	dataToDeserialize = SerializablePOD<int>::deserialize(dataToDeserialize, posZ);
	dataToDeserialize = SerializablePOD<char*>::deserialize(dataToDeserialize, message);
}