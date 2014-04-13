#include "depot/inc/Thing.h"

#include <gtest/gtest.h>        

using namespace ::testing;
using depot::Thing;

//TODO: needs   

struct ThingTest: public Test
{
    Thing t;
};

TEST_F(ThingTest, ThingCreatedShouldNotHaveEmptyName)
{
  ASSERT_NE(t.getName(), "");
}

TEST_F(ThingTest, ThingSetNameShouldNotAcceptEmptyName)
{
  ASSERT_THROW(t.setName(""), Thing::NameEmptyException);
}

TEST_F(ThingTest, ThingAfterSetNameShouldGetSameName)
{
  std::string name = "name";
  t.setName(name);
  ASSERT_EQ(t.getName(), name);
}