#include <gtest/gtest.h>
#include "../../../Code/depot/inc/DepotSerializer.h"
#include <sstream>

using namespace ::testing;
using namespace std::literals;

struct DepotSerializerTest : public Test
{
  depot::serialize::DepotSerializer serializer;
  auto expected_output = "Version: 1\n"s;
};

TEST_F(DepotSerializerTest, ShouldWriteVersionNumberWhenThereIsNoData)
{
  std::ostringstream output;
  serializer.serialize(output);
  EXPECT_EQ(expected_output, output.str());
}

TEST_F(DepotSerializerTest, ShouldWriteTopLevelArticle)
{

}