#include "gtest/gtest.h"
#include "User.h"

//shared resources
User *usr1;
User *usr2;

// The fixture for testing class UserTest.
class UserTest : public ::testing::Test {
protected:
	UserTest()
	{
		usr1 = usr2 = nullptr;
	}

	virtual ~UserTest()
	{
	}

	virtual void SetUp() {
		usr1 = new User();
		usr2 = new User();
	}

	virtual void TearDown() {
		delete usr1, usr2;
		usr1 = usr2 = nullptr;
	}
};

TEST_F(UserTest, TestUserInitialization) {
	EXPECT_EQ(strcmp(usr1->message, ""), 0);
	EXPECT_EQ(usr1->posX, 0);
	EXPECT_EQ(usr1->posY, 0);
	EXPECT_EQ(usr1->posZ, 0);
}

TEST_F(UserTest, TestUserSerializationAndDeserialiZation) {
	//set up data
	usr1->message = "Hello world, I'm getting serialized";
	usr1->posX = std::rand();
	usr1->posY = std::rand();
	usr1->posZ = std::rand();
	//serialize
	char* serializedData = usr1->serialize();
	//fill usr2 with usr1 data when deserializing
	usr2->deserialize(serializedData);
	EXPECT_EQ(strcmp(usr1->message, usr2->message), 0);
	EXPECT_EQ(usr1->posX, usr2->posX);
	EXPECT_EQ(usr1->posY, usr2->posY);
	EXPECT_EQ(usr1->posZ, usr2->posZ);
}