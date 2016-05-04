#include "gtest/gtest.h"
#include "User.h"

//shared resources
User *usr1;
User *usr2;

// The fixture for testing class UserTest.
class UserTest : public ::testing::Test
{
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
	EXPECT_EQ(strcmp(usr1->getMessage(), ""), 0);
	EXPECT_EQ(usr1->X, 0.0f);
	EXPECT_EQ(usr1->Y, 0.0f);
	EXPECT_EQ(usr1->Z, 0.0f);
}

TEST_F(UserTest, TestUserDeserializeAfterDelete) {
	//set up data
	usr1->setMessage("Hello world, I'm getting serialized");
	usr1->X = std::rand();
	usr1->Y = std::rand();
	usr1->Z = std::rand();

	float tempX = usr1->X;
	float tempY = usr1->Y;
	float tempZ = usr1->Z;
	char * mess = new char[strlen(usr1->getMessage())];
	strcpy(mess, usr1->getMessage());

	//serialize
	char* serializedData = usr1->serialize();
	//delete memory to ensure good copy
	usr1->deserialize(serializedData);
	delete usr1;
	usr1 = new User();

	//fill usr2 with usr1 data when deserializing
	usr2->deserialize(serializedData);
	EXPECT_EQ(strcmp(mess, usr2->getMessage()), 0);
	EXPECT_EQ(tempX, usr2->X);
	EXPECT_EQ(tempY, usr2->Y);
	EXPECT_EQ(tempZ, usr2->Z);
}

TEST_F(UserTest, TestUserSerializationAndDeserialiZation) {
	//set up data
	usr1->setMessage("Hello world, I'm getting serialized");
	usr1->X = std::rand();
	usr1->Y = std::rand();
	usr1->Z = std::rand();
	//serialize
	char* serializedData = usr1->serialize();
	//fill usr2 with usr1 data when deserializing
	usr2->deserialize(serializedData);
	EXPECT_EQ(strcmp(usr1->getMessage(), usr2->getMessage()), 0);
	EXPECT_EQ(usr1->X, usr2->X);
	EXPECT_EQ(usr1->Y, usr2->Y);
	EXPECT_EQ(usr1->Z, usr2->Z);
}

TEST_F(UserTest, TestUserSerializationAndDeserialiZationNullMessage) {
	//set up data
	usr1->setMessage(NULL);
	usr1->X = std::rand();
	usr1->Y = std::rand();
	usr1->Z = std::rand();
	//serialize
	char* serializedData = usr1->serialize();
	//fill usr2 with usr1 data when deserializing
	usr2->deserialize(serializedData);
	EXPECT_EQ(strcmp(usr1->getMessage(), usr2->getMessage()), 0);
	EXPECT_EQ(usr1->X, usr2->X);
	EXPECT_EQ(usr1->Y, usr2->Y);
	EXPECT_EQ(usr1->Z, usr2->Z);
}