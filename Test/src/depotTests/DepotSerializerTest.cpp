#include <gtest/gtest.h>
#include "../../../Code/depot/inc/DepotSerializer.h"
#include "../../../Code/depot/inc/Article.h"
#include "../../../Code/depot/inc/Container.h"
#include <sstream>

using namespace ::testing;
using namespace std::literals;

struct DepotSerializerTest : public Test
{
  depot::serialize::DepotSerializer serializer;
  std::string expected_output{"Version: 1\n"};

  void createTestSuiteArticles()
  {
    constexpr auto article_name = "Art1";
    constexpr auto article_unit = "Unit";
    const auto article = depot::Article::createTopLevelArticle(article_name, article_unit);
    constexpr auto second_article_name = "Art2";
    constexpr auto second_article_unit = "Unit";
    const auto second_article = depot::Article::createTopLevelArticle(second_article_name, second_article_unit);
    constexpr auto dependent_name = "dependent";
    constexpr auto dependent_unit = "dependent unit";
    const auto dependent_article = article->createDependentArticle(dependent_name, dependent_unit);
    constexpr auto dependent_second_name = "dependent_second";
    constexpr auto dependent_second_unit = "dependent unit_second";
    article->createDependentArticle(dependent_second_name, dependent_second_unit);
    constexpr auto dependent_dependent_name = "dependent_dependent";
    constexpr auto dependent_dependent_unit = "dependent_dependent unit";
    dependent_article->createDependentArticle(dependent_dependent_name, dependent_dependent_unit);
  }

  void expectReadTestSuiteArticles()
  {
    EXPECT_EQ(2U, depot::Article::getTopLevelArticles().size());
  }

  void createTestSuiteContainers()
  {
    constexpr auto container_name = "Container1";
    const auto container = depot::Container::createTopLevelContainer(container_name);
    constexpr auto second_container_name = "Container2";
    const auto second_container = depot::Container::createTopLevelContainer(second_container_name);
    constexpr auto dependent_name = "dependent";
    const auto dependent_container = container->createDependentContainer(dependent_name);
    constexpr auto dependent_second_name = "dependent_second";
    container->createDependentContainer(dependent_second_name);
    constexpr auto dependent_dependent_name = "dependent_dependent";
    dependent_container->createDependentContainer(dependent_dependent_name);
  }

  void expectReadTestSuiteContainers()
  {
    EXPECT_EQ(2U, depot::Container::getTopLevelContainers().size());
  }

  void clearDb()
  {
    depot::Article::clearTopLevelArticles();
    depot::Container::clearTopLevelContainers();
  }

  ~DepotSerializerTest()
  {
    depot::Article::clearTopLevelArticles();
    depot::Container::clearTopLevelContainers();
  }
};

TEST_F(DepotSerializerTest, ShouldWriteAndReadVersionNumberWhenThereIsNoData)
{
  std::stringstream stream;
  serializer.serialize(stream);
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
  createTestSuiteArticles();
  std::stringstream output;
  serializer.serialize(output);

  clearDb();
  serializer.deserialize(output);
  expectReadTestSuiteArticles();
}

TEST_F(DepotSerializerTest, SholdWriteAllContainers)
{
  createTestSuiteContainers();
  std::stringstream output;
  serializer.serialize(output);

  clearDb();
  serializer.deserialize(output);
  expectReadTestSuiteContainers();
}

TEST_F(DepotSerializerTest, ShouldWriteAllLevelArticlesAndAllLevelContainers)
{
  createTestSuiteArticles();
  createTestSuiteContainers();

  std::stringstream output;
  serializer.serialize(output);

  clearDb();

  serializer.deserialize(output);
  expectReadTestSuiteArticles();
  expectReadTestSuiteContainers();
}
