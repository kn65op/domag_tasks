#include <gtest/gtest.h>
#include "../../../Code/depot/inc/DepotSerializer.h"
#include <sstream>

using namespace ::testing;

struct DepotSerializerTest : public Test
{
  depot::serialize::DepotSerializer serializer;
};

TEST_F(DepotSerializerTest, D)
{
  std::ostringstream output;
  serializer.serialize(output);
}