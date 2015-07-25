#include <gtest/gtest.h>
#include "../../../Code/depot/inc/DepotSerializer.h"
#include "../../../Code/depot/inc/Article.h"
#include "../../../Code/depot/inc/Container.h"
#include <sstream>


#include <iostream>
using namespace ::testing;
using namespace std::literals;

struct DepotSerializerTest : public Test
{
  depot::serialize::DepotSerializer serializer;
  std::string expected_output{"Version: 1\n"};

  void TearDown() override
  {
    depot::Article::clearTopLevelArticles();
    depot::Container::clearTopLevelContainers();
  }
};

TEST_F(DepotSerializerTest, ShouldWriteAndReadVersionNumberWhenThereIsNoData)
{
  std::stringstream stream;
  serializer.serialize(stream);
  std::cout << stream.str() << "\n";
  serializer.deserialize(stream);
  EXPECT_EQ(0, depot::Article::getTopLevelArticles().size());
  EXPECT_EQ(0, depot::Container::getTopLevelContainers().size());
}

TEST_F(DepotSerializerTest, ShouldNotReadWhenThereIsNoVersion)
{
  std::istringstream input;
  EXPECT_THROW(serializer.deserialize(input), depot::serialize::DepotSerializer::InvalidVersion);
}

TEST_F(DepotSerializerTest, ShouldNotReadWhenVersionIs0or2)
{
  std::istringstream input_version_zero{"Version: 0"};
  std::istringstream input_version_two{"Version: 2"};

  EXPECT_THROW(serializer.deserialize(input_version_zero), depot::serialize::DepotSerializer::InvalidVersion);
  EXPECT_THROW(serializer.deserialize(input_version_two), depot::serialize::DepotSerializer::InvalidVersion);
}

TEST_F(DepotSerializerTest, ShouldWriteAllLevelArticles)
{
  std::stringstream output;

  const auto article_name = "Art1"s;
  const auto article_unit = "Unit"s;
  const auto article = depot::Article::createTopLevelArticle(article_name, article_unit);
  const auto second_article_name = "Art2"s;
  const auto second_article_unit = "Unit"s;
  const auto second_article = depot::Article::createTopLevelArticle(second_article_name, second_article_unit);
  const auto dependent_name = "dependent"s;
  const auto dependent_unit = "dependent unit"s;
  const auto dependent_article = article->createDependentArticle(dependent_name, dependent_unit);
  const auto dependent_second_name = "dependent_second"s;
  const auto dependent_second_unit = "dependent unit_second"s;
  const auto dependent_article_second = article->createDependentArticle(dependent_second_name, dependent_second_unit);
  const auto dependent_dependent_name = "dependent_dependent"s;
  const auto dependent_dependent_unit = "dependent_dependent unit"s;
  const auto dependent_dependent_articlearticle= dependent_article->createDependentArticle(dependent_dependent_name, dependent_dependent_unit);
  serializer.serialize(output);
  std::cout << output.str() << "\n";

  depot::Article::clearTopLevelArticles();
  serializer.deserialize(output);
  EXPECT_EQ(2U, depot::Article::getTopLevelArticles().size());

  /*
  expected_output += "Articles:\n";
  expected_output += "  - id: 1\n";
  expected_output += "    name: " + article_name + "\n";
  expected_output += "    unit: " + article_unit + "\n";
  expected_output += "    dependent_articles:\n";
  expected_output += "      - 2\n";
  expected_output += "      - 4\n";
  expected_output += "  - id: 2\n";
  expected_output += "    name: " + dependent_name + "\n";
  expected_output += "    unit: " + dependent_unit + "\n";
  expected_output += "    dependent_articles:\n";
  expected_output += "      - 3\n";
  expected_output += "  - id: 3\n";
  expected_output += "    name: " + dependent_dependent_name + "\n";
  expected_output += "    unit: " + dependent_dependent_unit + "\n";
  expected_output += "  - id: 4\n";
  expected_output += "    name: " + dependent_second_name + "\n";
  expected_output += "    unit: " + dependent_second_unit + "\n";
  expected_output += "  - id: 5\n";
  expected_output += "    name: " + second_article_name + "\n";
  expected_output += "    unit: " + second_article_unit;
  */
//  EXPECT_EQ(expected_output, output.str());
}

TEST_F(DepotSerializerTest, SholdWriteAllContainers)
{
  std::ostringstream output;

  const auto container_name = "Container1"s;
  const auto container = depot::Container::createTopLevelContainer(container_name);
  const auto second_container_name = "Container2"s;
  const auto second_container = depot::Container::createTopLevelContainer(second_container_name);
  const auto dependent_name = "dependent"s;
  const auto dependent_container = container->createDependentContainer(dependent_name);
  const auto dependent_second_name = "dependent_second"s;
  const auto dependent_container_second = container->createDependentContainer(dependent_second_name);
  const auto dependent_dependent_name = "dependent_dependent"s;
  const auto dependent_dependent_containercontainer= dependent_container->createDependentContainer(dependent_dependent_name);

  serializer.serialize(output);

  expected_output += "Containers:\n";
//  EXPECT_EQ(expected_output, output.str());
}