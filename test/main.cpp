#include "gtest/gtest.h"
#include "User.h"

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	int ret = RUN_ALL_TESTS();
	return ret;
}

TEST(TestMemcpy, TestCharCopy) {
	const char * char1 = "Hola mundo";
	char * buffer = new char[50];
	//char buffer[50];
	memcpy(buffer, char1, strlen(char1) + 1);
};

TEST(UserTest, ValuesInitialization) {
	User usr1;
	User usr2;

	ASSERT_EQ(usr1.posX, 0);
	ASSERT_EQ(usr1.posY, 0);
	ASSERT_EQ(usr1.posZ, 0);

	usr1.posX = 1;
	usr1.posY = 2;
	usr1.posZ = 3;
	usr1.message = "Hola mundo";
	int length = strlen(usr1.message);
	char*  data = usr1.serialize();
	usr2.deserialize(data);

	//ASSERT_EQ(usr.posX, usr2.posX);
	//ASSERT_EQ(usr.posY, usr2.posY);
	//ASSERT_EQ(usr.posZ, usr2.posZ);
	//int cmp = strcmp(usr.message, usr2.message);
	//ASSERT_EQ(cmp, 0);

//	delete data;
};