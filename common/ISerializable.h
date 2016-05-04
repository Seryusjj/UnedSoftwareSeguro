#pragma once

#include <cstdint>

class ISerializable //interface
{
public:

	//return the total size of the serialized object
	virtual int32_t serializeSize() const = 0;

	//return the char* corresponding to the serialized data of the calling object
	virtual char*  serialize() = 0;

	//get the data char* and deserilize into the calling object
	virtual void deserialize(const char* dataToDeserialize) = 0;
};
