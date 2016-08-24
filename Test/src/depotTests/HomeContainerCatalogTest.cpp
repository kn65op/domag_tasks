#include "gtest/gtest.h"

#include "HomeContainerCatalog.h"

using namespace ::testing;

struct HomeContainerCatalogTest : public Test
{
  depot::HomeContainerCatalog catalog;
};

TEST_F(HomeContainerCatalogTest, D)
{
  ASSERT_TRUE(true);
}