#include <gtest/gtest.h>
#include "DepotSerializer.h"
#include "Article.h"
#include "Item.h"
#include "Container.h"
#include <sstream>

using namespace ::testing;
using namespace std::literals;

struct DepotSerializerTest : public Test
{
  depot::serialize::DepotSerializer serializer;
  const std::string article_name = "Z art with item";
  const std::string container_name = "Z container with item";
  const double itemQuantity{5.88};
  const double itemPrice{123.43};
  const double itemPricePerUnit{itemPrice / itemQuantity};
  const double firstConsume = 0.98;
  const double secondConsume = 2.3;
  const double amountAfterConsumption = 2.6;
  const boost::gregorian::date boughtDay = boost::gregorian::day_clock::local_day() - boost::gregorian::date_duration(7);
  const boost::gregorian::date firstConsumeTime = boost::gregorian::day_clock::local_day() - boost::gregorian::date_duration(5);
  const boost::gregorian::date secondConsumeTime = boost::gregorian::day_clock::local_day() - boost::gregorian::date_duration(1);

  void createTestSuiteArticles()
  {
    constexpr auto second_article_name = "Art 2";
    constexpr auto second_article_unit = "Unit";
    const auto second_article = depot::Article::createTopLevelArticle(second_article_name, second_article_unit);
    constexpr auto article_unit = "Unit";
    const auto article = depot::Article::createTopLevelArticle(article_name, article_unit);
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
    constexpr auto second_container_name = "Container2";
    const auto second_container = depot::Container::createTopLevelContainer(second_container_name);
    const auto container = depot::Container::createTopLevelContainer(container_name);
    constexpr auto dependent_name = "dependent";
    const auto dependent_container = container->createDependentContainer(dependent_name);
    constexpr auto dependent_second_name = "dependent_second";
    container->createDependentContainer(dependent_second_name);
    constexpr auto dependent_dependent_name = "dependent_dependent";
    dependent_container->createDependentContainer(dependent_dependent_name);
  }

  void addItemsToContainers()
  {
    const auto articles = depot::Article::getTopLevelArticles();
    auto art = *std::find_if(articles.begin(), articles.end(), [this](const auto article)
    {
     return article->getName() == article_name;
    });
    auto item = std::make_unique<depot::Item>(art);
    item->buy(itemQuantity, itemPrice, boughtDay);
    item->consume(firstConsume, firstConsumeTime);
    item->consume(secondConsume, secondConsumeTime);

    const auto containers = depot::Container::getTopLevelContainers();
    auto cont = *std::find_if(containers.begin(), containers.end(), [this](const auto container)
    {
     return container->getName() == container_name;
    });
    cont->addItem(std::move(item));
  }

  void expectItemInContainer()
  {
    const auto containers = depot::Container::getTopLevelContainers();
    auto cont = *std::find_if(containers.begin(), containers.end(), [this](const auto container)
    {
      return container->getName() == container_name;
    });
    ASSERT_FALSE(cont->getItems().empty());
    auto item = cont->getItems().front();
    EXPECT_DOUBLE_EQ(itemQuantity, item->getBoughtAmmount());
    EXPECT_DOUBLE_EQ(amountAfterConsumption, item->getQuantity());
    EXPECT_DOUBLE_EQ(itemPricePerUnit, item->getPricePerUnit());
    EXPECT_EQ(boughtDay, item->getBuyDate());
    EXPECT_DOUBLE_EQ(firstConsume, item->getConsumeHistory()[0].first);
    EXPECT_DOUBLE_EQ(secondConsume, item->getConsumeHistory()[1].first);
    EXPECT_EQ(firstConsumeTime, item->getConsumeHistory()[0].second);
    EXPECT_EQ(secondConsumeTime, item->getConsumeHistory()[1].second);
    EXPECT_EQ(article_name, item->getThing().lock()->getName());
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
    clearDb();
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

TEST_F(DepotSerializerTest, ShouldWriteAllItemsInContainersAndArticles)
{
  createTestSuiteArticles();
  createTestSuiteContainers();
  addItemsToContainers();

  std::stringstream output;
  EXPECT_NO_THROW(serializer.serialize(output));

  std::cout << output.str() << "\n";
  clearDb();

  serializer.deserialize(output);
  expectReadTestSuiteArticles();
  expectReadTestSuiteContainers();
  expectItemInContainer();
}