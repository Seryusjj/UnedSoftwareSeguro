#include "gtest/gtest.h"
#include "ConfigurationManager.h"

// The fixture for testing class ConfigurationManagerTest.
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
	manager.loadDataOfFile("testFile.properties");
	int port = manager.getValueOfPropertie<int>("test.port");
	std::string addr = manager.getValueOfPropertie<std::string>("test.address");
	std::string addr1 = manager.getValueOfPropertie<std::string>("test.address1");

	EXPECT_EQ(port, 4000);
	EXPECT_EQ(addr, "127.0.0.1");
	EXPECT_EQ(addr, "127.0.0.1");
}

TEST_F(ConfigurationManagerTest, ReadNonExistingConfigFileTest)
{
	try
	{
		ConfigurationManager& manager = ConfigurationManager::getInstance();
		manager.loadDataOfFile("wrong.properties");
		FAIL();
	}
	catch (std::exception const & err)
	{
		EXPECT_EQ(err.what(), std::string("Error reading config file, may be the path or filename are wrong"));
	}
}