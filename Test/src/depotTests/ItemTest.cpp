#include "gtest/gtest.h"
#include "depot/inc/Item.h"

using namespace ::testing;
using depot::Item;

struct ItemTest : public Test
{
  Item i;
};

TEST_F(ItemTest, testCreate)
{
}