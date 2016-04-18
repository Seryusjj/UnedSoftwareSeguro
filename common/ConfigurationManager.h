#pragma once
#include "DLLDefines.h"
#include <string>
#include <map>

class EXPORT FileReadException : public std::exception
{
public:
	virtual const char* what() const throw()
	{
		return "Error reading config file, may be the path or filename are wrong";
	}
};

class EXPORT ConfigurationManager
{
public:
	static ConfigurationManager& getInstance()
	{
		static ConfigurationManager  instance; // Guaranteed to be destroyed.
		return instance; // Instantiated on first use.
	}
private:
	ConfigurationManager() {};
	FileReadException  fileReadException;

public:

	ConfigurationManager(ConfigurationManager const&) = delete;
	void operator=(ConfigurationManager const&) = delete;

	std::map<std::string, std::string> ReadConfigFile(std::string const& filename);
};