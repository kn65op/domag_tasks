#include <iostream>

#include <gtest/gtest.h>

TEST(Test, TestTrue)
{
  ASSERT_TRUE(true);
}

TEST(Test, TestFalse)
{
	ASSERT_TRUE(false);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
