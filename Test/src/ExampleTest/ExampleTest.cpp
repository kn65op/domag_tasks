#include <iostream>
#include "GMockTetsMocks/AdderMock.h"

using namespace ::testing;

TEST(Test, TestTrue)
{
  ASSERT_TRUE(true);
}

TEST(Test, TestFalse)
{
	ASSERT_FALSE(false);
}

TEST(MockTest, AdderTest)
{
	AdderMock adder;
	EXPECT_CALL(adder, add(1,2)).WillOnce(Return(3));

	ASSERT_EQ(3, adder.add(1, 2));
}
