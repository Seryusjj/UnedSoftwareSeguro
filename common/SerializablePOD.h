#pragma once
#include <cstring>

//serialize POD types like int float char*, struct or the keyword class without
//constructors, destructors and virtual members functions.
template <typename T> class SerializablePOD
{
public:
	static size_t serializeSize(T str)
	{
		return sizeof(T);
	}

	static char* serialize(char* target, T value)
	{
		memcpy(target, &value, serializeSize(value));
		return target + serializeSize(value);
	}

	static const char* deserialize(const char* source, T& target)
	{
		memcpy(&target, source, serializeSize(target));
		return source + serializeSize(target);
	}
};

template<> size_t SerializablePOD<char*>::serializeSize(char* str)
{
	if (str == NULL) return 0;
	size_t result = strlen(str) + 1;
	result += sizeof(size_t);
	return result;
}

template<> char* SerializablePOD<char*>::serialize(char* target, char* value)
{
	//serialize the string size
	size_t lenght = strlen(value) + 1;;

	memcpy(target, &lenght, sizeof(size_t));
	//serialize the string
	memcpy(target + sizeof(size_t), value, lenght);
	return target + lenght + sizeof(size_t);
}

template<> const char* SerializablePOD<char*>::deserialize(const char* source, char*& target)
{
	size_t length;
	//deserialize the string size
	memcpy(&length, source, sizeof(size_t));
	//deserialize the string
	target = new char[length];
	memcpy(target, source + sizeof(size_t), length);

	return source + sizeof(size_t) + length;
}
