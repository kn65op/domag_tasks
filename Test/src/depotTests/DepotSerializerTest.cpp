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

TEST_F(DepotSerializerTest, ShouldWriteAllLevelArticles)
{
  std::ostringstream output;

  const auto article_name = "Art1"s;
  const auto article_unit = "Unit"s;
  const auto article = depot::TopLevelArticles::createTopLevelArticle(article_name, article_unit);
  const auto second_article_name = "Art2"s;
  const auto second_article_unit = "Unit"s;
  const auto second_article = depot::TopLevelArticles::createTopLevelArticle(second_article_name, second_article_unit);
  const auto dependent_name = "dependent"s;
  const auto dependent_unit = "dependent unit"s;
  const auto dependent_article = depot::Article::createDependentArticle(article, dependent_name, dependent_unit);
  const auto dependent_dependent_name = "dependent_dependent"s;
  const auto dependent_dependent_unit = "dependent_dependent unit"s;
  const auto dependent_dependent_articlearticle= depot::Article::createDependentArticle(dependent_article, dependent_dependent_name, dependent_dependent_unit);
  serializer.serialize(output);

  expected_output += "Articles:\n";
  expected_output += "  - id: 1\n";
  expected_output += "    name: " + article_name + "\n";
  expected_output += "    unit: " + article_unit + "\n";
  expected_output += "  - id: 2\n";
  expected_output += "    name: " + dependent_name + "\n";
  expected_output += "    unit: " + dependent_unit + "\n";
  expected_output += "  - id: 3\n";
  expected_output += "    name: " + dependent_dependent_name + "\n";
  expected_output += "    unit: " + dependent_dependent_unit + "\n";
  expected_output += "  - id: 4\n";
  expected_output += "    name: " + second_article_name + "\n";
  expected_output += "    unit: " + second_article_unit;
  EXPECT_EQ(expected_output, output.str());
}