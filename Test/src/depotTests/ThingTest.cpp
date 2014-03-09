#include "depot/inc/Thing.h"

#include <gtest/gtest.h>        

using namespace ::testing;

struct ThingTests : public Test
{
    Thing t;
};

TEST_F(ThingTests, ThingCreatedShouldNotHaveEmptyName)
{
  ASSERT_NE(t.getName(), "");
}

TEST_F(ThingTests, ThingSetNameShouldNotAcceptEmptyName)
{
  ASSERT_THROW(t.setName(""), Thing::NameEmptyException);
}