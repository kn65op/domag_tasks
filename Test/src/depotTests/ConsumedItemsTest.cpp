#include "gtest/gtest.h"

#include "ConsumedItems.hpp"

using namespace ::testing;

struct ConsumedItemsTest : public Test
{
  depot::ConsumedItems items;
};

TEST_F(ConsumedItemsTest, D)
{
  ASSERT_TRUE(true);
}
