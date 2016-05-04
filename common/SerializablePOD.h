#pragma once
#include <cstring>

//serialize POD types like int float char*, struct or the keyword class without
//constructors, destructors and virtual members functions.
template <typename T> class SerializablePOD
{
public:
	static int32_t serializeSize(T str)
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

template<> int32_t SerializablePOD<char*>::serializeSize(char* str)
{
	if (str == NULL) return 0;
	int32_t result = strlen(str) + 1;//add 1 to copy the \0 termination
	result += sizeof(int32_t);
	return result;
}

template<> char* SerializablePOD<char*>::serialize(char* target, char* value)
{
	//serialize the string size
	int32_t lenght = strlen(value) + 1;;

	memcpy(target, &lenght, sizeof(int32_t));
	//serialize the string
	memcpy(target + sizeof(int32_t), value, lenght);
	return target + lenght + sizeof(int32_t);
}

template<> const char* SerializablePOD<char*>::deserialize(const char* source, char*& target)
{
	int32_t length;
	//deserialize the string size
	memcpy(&length, source, sizeof(int32_t));
	//deserialize the string
	//target = new char[length];
	delete[] target;
	target = new char[length];
	memcpy(target, source + sizeof(int32_t), length);
	return source + sizeof(int32_t) + length;
}
