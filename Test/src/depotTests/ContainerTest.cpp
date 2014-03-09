#include "gtest/gtest.h"
#include "depot/inc/Container.h"

using namespace ::testing;
using depot::Container;

struct ContainerTest : public Test
{
  Container c;
};

TEST_F(ContainerTest, testCreate)
{
}