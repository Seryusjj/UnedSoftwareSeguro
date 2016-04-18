#include "ConfigurationManager.h"
#include <fstream>
#include <exception>

std::string trim(std::string const& source, char const* delims = " \t\r\n") {
	std::string result(source);
	std::string::size_type index = result.find_last_not_of(delims);
	if (index != std::string::npos)
		result.erase(++index);

	index = result.find_first_not_of(delims);
	if (index != std::string::npos)
		result.erase(0, index);
	else
		result.erase();
	return result;
}

std::map<std::string, std::string> ConfigurationManager::ReadConfigFile(std::string const& configFile)
{
	std::ifstream file(configFile.c_str());
	bool fail = file.fail();
	if (fail) {
		throw fileReadException;
	}

	std::map<std::string, std::string> _content;
	std::string line;
	std::string name;
	std::string value;
	std::string inSection;
	int posEqual;
	while (std::getline(file, line)) {
		if (!line.length()) continue;
		//comments
		if (line[0] == '#') continue;

		posEqual = line.find('=');
		name = trim(line.substr(0, posEqual));
		value = trim(line.substr(posEqual + 1));

		_content[name] = value;
	}
	return _content;
}