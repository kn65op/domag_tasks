#include "gtest/gtest.h"

#include "HomeContainerCatalog.h"

using namespace ::testing;

struct HomeContainerCatalogTest : public Test
{
  depot::HomeContainerCatalog catalog;
};

TEST_F(HomeContainerCatalogTest, CanCreateAndRemoveTopLevenContainer)
{
  EXPECT_EQ(0U, catalog.getTopLevelContainers().size());
  const auto c = catalog.createTopLevelContainer();
  ASSERT_EQ(1U, catalog.getTopLevelContainers().size());
  catalog.removeTopLevelContainer(c);
  EXPECT_EQ(0U, catalog.getTopLevelContainers().size());
}

TEST_F(HomeContainerCatalogTest, AfterCleaningTopLevelEntities)
{
  EXPECT_EQ(0U, catalog.getTopLevelContainers().size());
catalog.createTopLevelContainer();
  ASSERT_EQ(1U, catalog.getTopLevelContainers().size());
  catalog.clearTopLevelContainers();
  EXPECT_EQ(0U, catalog.getTopLevelContainers().size());
}