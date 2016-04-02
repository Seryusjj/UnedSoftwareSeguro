#include "gtest/gtest.h"
#include "User.h"

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	int ret = RUN_ALL_TESTS();
	return ret;
}

TEST(UserTest, ValuesInitialization) {
	User usr;

	ASSERT_EQ(usr.posX, 0);
	ASSERT_EQ(usr.posY, 0);
	ASSERT_EQ(usr.posZ, 0);
}