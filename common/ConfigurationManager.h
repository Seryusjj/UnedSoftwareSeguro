#pragma once
#include <string>

class ConfigurationManager
{
public:
	static ConfigurationManager& getInstance()
	{
		static ConfigurationManager  instance; // Guaranteed to be destroyed.
		return instance; // Instantiated on first use.
	}
private:
	ConfigurationManager() {};
	ConfigurationManager(ConfigurationManager const&);              // Don't Implement
	void operator=(ConfigurationManager const&); // Don't implement

public:
	ConfigurationManager(ConfigurationManager const&) = delete;
	void operator=(ConfigurationManager const&) = delete;

private:
	void ReadConfigFile(std::string filename);
};