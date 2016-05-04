#pragma once
#include "DLLDefines.h"
#include <string>
#include <fstream>
#include <map>

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
	std::map<std::string, std::string> _properties;

public:
	ConfigurationManager(ConfigurationManager const&) = delete;
	void operator=(ConfigurationManager const&) = delete;

	template<typename T>
	inline T getValueOfPropertie(std::string key);

	void loadDataOfFile(std::string const& filename);
};

template<typename T>
inline T ConfigurationManager::getValueOfPropertie(std::string key)
{
	std::stringstream ss(_properties[key]);
	T value;
	ss >> value;
	return value;
}