#include "gtest/gtest.h"
#include "ConfigurationManager.h"
#include <direct.h>
// The fixture for testing class UserTest.
class ConfigurationManagerTest : public ::testing::Test {
protected:
	ConfigurationManagerTest()
	{
	}

	virtual ~ConfigurationManagerTest()
	{
	}

	virtual void SetUp() {
	}

	virtual void TearDown() {
	}
};

TEST_F(ConfigurationManagerTest, ReadConfigFileTest)
{
	ConfigurationManager& manager = ConfigurationManager::getInstance();
	std::map<std::string, std::string>& configFileValues = manager.ReadConfigFile("testFile.properties");
	std::string port = configFileValues["test.port"];
	std::string addr = configFileValues["test.address"];
	std::string addr1 = configFileValues["test.address1"];

	EXPECT_EQ(port, "4000");
	EXPECT_EQ(addr, "127.0.0.1");
	EXPECT_EQ(addr, "127.0.0.1");
}

TEST_F(ConfigurationManagerTest, ReadNonExistingConfigFileTest)
{
	try
	{
		ConfigurationManager& manager = ConfigurationManager::getInstance();
		std::map<std::string, std::string>& configFileValues = manager.ReadConfigFile("wrong.properties");
		FAIL();
	}
	catch (FileReadException const & err)
	{
		EXPECT_EQ(err.what(), std::string("Error reading config file, may be the path or filename are wrong"));
	}
}