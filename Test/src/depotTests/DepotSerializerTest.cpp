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

  auto article_name = "Art1"s;
  auto article_unit = "Unit"s;
  auto article = depot::TopLevelArticles::createTopLevelArticle(article_name, article_unit);
  auto second_article_name = "Art2"s;
  auto second_article_unit = "Unit"s;
  auto second_article = depot::TopLevelArticles::createTopLevelArticle(second_article_name, second_article_unit);
  auto dependent_name = "dependent"s;
  auto dependent_unit = "dependent unit"s;
  auto dependent_article = depot::Article::createDependentArticle(article, dependent_name, dependent_unit);
  auto dependent_dependent_name = "dependent_dependent"s;
  auto dependent_dependent_unit = "dependent_dependent unit"s;
  auto dependent_dependent_articlearticle= depot::Article::createDependentArticle(dependent_article, dependent_dependent_name, dependent_dependent_unit);
  serializer.serialize(output);

  expected_output += "Articles:\n";
  expected_output += "  - name: " + article_name + "\n";
  expected_output += "    unit: " + article_unit + "\n";
  expected_output += "  - name: " + dependent_name + "\n";
  expected_output += "    unit: " + dependent_unit + "\n";
  expected_output += "  - name: " + dependent_dependent_name + "\n";
  expected_output += "    unit: " + dependent_dependent_unit + "\n";
  expected_output += "  - name: " + second_article_name + "\n";
  expected_output += "    unit: " + second_article_unit;
  EXPECT_EQ(expected_output, output.str());
}