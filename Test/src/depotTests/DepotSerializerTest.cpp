#include <gtest/gtest.h>
#include "../../../Code/depot/inc/DepotSerializer.h"
#include "../../../Code/depot/inc/Article.h"
#include <sstream>

using namespace ::testing;
using namespace std::literals;

struct DepotSerializerTest : public Test
{
  depot::serialize::DepotSerializer serializer;
  std::string expected_output{"Version: 1\n"};

  void TearDown() override
  {
    depot::TopLevelArticles::clearTopLevelArticles();
  }
};

TEST_F(DepotSerializerTest, ShouldWriteVersionNumberWhenThereIsNoData)
{
  std::ostringstream output;
  serializer.serialize(output);
  EXPECT_EQ(expected_output, output.str());
}

TEST_F(DepotSerializerTest, ShouldWriteTopLevelArticle)
{
  std::ostringstream output;

  auto article = depot::TopLevelArticles::createTopLevelArticle();
  serializer.serialize(output);

  expected_output += "Article\n";
  EXPECT_EQ(expected_output, output.str());
}