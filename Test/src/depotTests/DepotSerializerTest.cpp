#include <gtest/gtest.h>
#include "DepotSerializer.h"
#include "Article.h"
#include "Item.h"
#include "Container.h"
#include "depot/inc/HomeContainerCatalog.h"
#include <sstream>

#include "TLogger.h"

using namespace ::testing;
using namespace std::literals;

struct DepotSerializerTest : public Test
{
  depot::HomeContainerCatalog catalog;
  depot::serialize::DepotSerializer serializer;
  const std::string article_name = "Z art with item";
  const std::string second_article_name = "secondArticle name";
  const std::string articleNameThird = "Third article with item";
  const std::string container_name = "Z container with item";
  const std::string containerNameSecond = "Second container with item";
  const double itemQuantity{5.88};
  const double itemPrice{123.43};
  const double itemPricePerUnit{itemPrice / itemQuantity};
  const double secondItemQuantity{10.11};
  const double secondItemPrice{13.43};
  const double secondItemPricePerUnit{secondItemPrice / secondItemQuantity};
  const double itemQuantityThird{128.7};
  const double itemPriceThird{100};
  const double itemPricePerUnitThird{itemPriceThird / itemQuantityThird};
  const double firstConsume = 0.98;
  const double secondConsume = 2.3;
  const double thirdConsume = 3.3;
  const double fourthConsume = 25.7;
  const double fifthConsume = 25.3;
  const double amountAfterConsumption = 2.6;
  const double secondAmountAfterConsumption = 5.83;
  const double amountAfterConsumptionThird = 77.7;
  const boost::gregorian::date bestBeforeDateOne = boost::gregorian::day_clock::local_day() + boost::gregorian::date_duration(7);
  const boost::gregorian::date bestBeforeDateTwo = boost::gregorian::day_clock::local_day() + boost::gregorian::date_duration(77);
  const boost::gregorian::date boughtDay = boost::gregorian::day_clock::local_day() - boost::gregorian::date_duration(7);
  const boost::gregorian::date secondBoughtDay = boost::gregorian::day_clock::local_day() - boost::gregorian::date_duration(27);
  const boost::gregorian::date firstConsumeTime = boost::gregorian::day_clock::local_day() - boost::gregorian::date_duration(5);
  const boost::gregorian::date secondConsumeTime = boost::gregorian::day_clock::local_day() - boost::gregorian::date_duration(1);
  const boost::gregorian::date thirdConsumeTime = boost::gregorian::day_clock::local_day() - boost::gregorian::date_duration(4);
  depot::ItemsContainer *containerForThirdItem;
  std::shared_ptr<depot::IArticle> articleForThirdItem;

  void createTestSuiteArticles()
  {
    constexpr auto second_article_unit = "Unit";
    const auto second_article = depot::Article::createTopLevelArticle(second_article_name, second_article_unit);
    constexpr auto article_unit = "Unit";
    const auto article = depot::Article::createTopLevelArticle(article_name, article_unit);
    constexpr auto dependent_name = "dependent_second";
    constexpr auto dependent_unit = "dependent unit";
    const auto dependent_article = article->createDependentArticle(dependent_name, dependent_unit);
    constexpr auto dependent_second_unit = "dependent unit_second";
    articleForThirdItem = article->createDependentArticle(articleNameThird, dependent_second_unit);
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
    const auto second_container = catalog.createTopLevelContainer(second_container_name);
    const auto container = catalog.createTopLevelContainer(container_name);
    constexpr auto dependent_name = "dependent";
    const auto dependent_container = container->createDependentContainer(dependent_name);
    containerForThirdItem = container->createDependentContainer(containerNameSecond).get();
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
    auto item = std::make_unique<depot::Item>(art, depot::PurcaseDetails{itemQuantity, itemPrice, boughtDay}, bestBeforeDateOne);
    item->consume(firstConsume, firstConsumeTime);
    item->consume(secondConsume, secondConsumeTime);

    const auto containers = catalog.getTopLevelContainers();
    auto cont = *std::find_if(containers.begin(), containers.end(), [this](const auto container)
    {
     return container->getName() == container_name;
    });
    cont->addItem(std::move(item));

    auto itemSecond = std::make_unique<depot::Item>(articleForThirdItem, depot::PurcaseDetails{itemQuantityThird, itemPriceThird, boughtDay});
    itemSecond->consume(fifthConsume, secondConsumeTime);
    itemSecond->consume(fourthConsume, thirdConsumeTime);
    containerForThirdItem->addItem(std::move(itemSecond));
  }

  void expectItemInContainer()
  {
    const auto containers = catalog.getTopLevelContainers();
    auto cont = *std::find_if(containers.begin(), containers.end(), [this](const auto container)
    {
      return container->getName() == container_name;
    });
    ASSERT_FALSE(cont->getItems().empty());
    auto item = cont->getItems().front();
    EXPECT_DOUBLE_EQ(itemQuantity, item->getBoughtAmount());
    EXPECT_DOUBLE_EQ(amountAfterConsumption, item->getQuantity());
    EXPECT_DOUBLE_EQ(itemPricePerUnit, item->getPricePerUnit());
    EXPECT_EQ(boughtDay, item->getBuyDate());
    EXPECT_EQ(bestBeforeDateOne, item->getBestBefore());
    EXPECT_DOUBLE_EQ(firstConsume, item->getConsumeHistory()[0].first);
    EXPECT_DOUBLE_EQ(secondConsume, item->getConsumeHistory()[1].first);
    EXPECT_EQ(firstConsumeTime, item->getConsumeHistory()[0].second);
    EXPECT_EQ(secondConsumeTime, item->getConsumeHistory()[1].second);
    EXPECT_EQ(article_name, item->getThing().lock()->getName());

    auto contSecond = *std::find_if(cont->getContainers().begin(), cont->getContainers().end(), [this](const auto container)
    {
      return container->getName() == containerNameSecond;
    });
    ASSERT_FALSE(contSecond->getItems().empty());
    auto itemSecond = contSecond->getItems().front();
    EXPECT_DOUBLE_EQ(itemQuantityThird, itemSecond->getBoughtAmount());
    EXPECT_DOUBLE_EQ(amountAfterConsumptionThird, itemSecond->getQuantity());
    EXPECT_DOUBLE_EQ(itemPricePerUnitThird, itemSecond->getPricePerUnit());
    EXPECT_EQ(boughtDay, itemSecond->getBuyDate());
    EXPECT_FALSE(itemSecond->getBestBefore());
    EXPECT_DOUBLE_EQ(fourthConsume, itemSecond->getConsumeHistory()[0].first);
    EXPECT_DOUBLE_EQ(fifthConsume, itemSecond->getConsumeHistory()[1].first);
    EXPECT_EQ(thirdConsumeTime, itemSecond->getConsumeHistory()[0].second);
    EXPECT_EQ(secondConsumeTime, itemSecond->getConsumeHistory()[1].second);
    EXPECT_EQ(articleNameThird, itemSecond->getThing().lock()->getName());
  }

  void addItemsToConsumedItems()
  {
    const auto articles = depot::Article::getTopLevelArticles();
    auto art = *std::find_if(articles.begin(), articles.end(), [this](const auto article)
    {
     return article->getName() == second_article_name;
    });
    auto item = std::make_unique<depot::Item>(art, depot::PurcaseDetails{secondItemQuantity, secondItemPrice, secondBoughtDay}, bestBeforeDateTwo);
    item->consume(firstConsume, firstConsumeTime);
    item->consume(thirdConsume, thirdConsumeTime);

    auto &consumedItems = catalog.getContainerForConsumedItems();
    consumedItems.addItem(std::move(item));
  }

  void expectItemInConsumedItems()
  {
    auto& consumedItems = catalog.getContainerForConsumedItems();
    const auto& items = consumedItems.getItems();
    ASSERT_FALSE(items.empty());
    const auto& item = items.front();
    EXPECT_DOUBLE_EQ(secondItemQuantity, item->getBoughtAmount());
    EXPECT_DOUBLE_EQ(secondAmountAfterConsumption, item->getQuantity());
    EXPECT_DOUBLE_EQ(secondItemPricePerUnit, item->getPricePerUnit());
    EXPECT_EQ(secondBoughtDay, item->getBuyDate());
    EXPECT_EQ(bestBeforeDateTwo, item->getBestBefore());
    EXPECT_DOUBLE_EQ(firstConsume, item->getConsumeHistory()[0].first);
    EXPECT_DOUBLE_EQ(thirdConsume, item->getConsumeHistory()[1].first);
    EXPECT_EQ(firstConsumeTime, item->getConsumeHistory()[0].second);
    EXPECT_EQ(thirdConsumeTime, item->getConsumeHistory()[1].second);
    EXPECT_EQ(second_article_name, item->getThing().lock()->getName());
  }

  void expectReadTestSuiteContainers()
  {
    EXPECT_EQ(2U, catalog.getTopLevelContainers().size());
  }

  void clearDb()
  {
    depot::Article::clearTopLevelArticles();
    catalog.clearAllContainers();
    articleForThirdItem = nullptr;
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
  EXPECT_EQ(0, catalog.getTopLevelContainers().size());
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
  addItemsToConsumedItems();

  std::stringstream output;
  EXPECT_NO_THROW(serializer.serialize(output));

  LOG << output.str();
  clearDb();

  serializer.deserialize(output);
  expectReadTestSuiteArticles();
  expectReadTestSuiteContainers();
  expectItemInContainer();
  expectItemInConsumedItems();
}

TEST_F(DepotSerializerTest, ShouldWriteAllItemsInContainersAndArticlesAndReadTwice)
{
  createTestSuiteArticles();
  createTestSuiteContainers();
  addItemsToContainers();
  addItemsToConsumedItems();

  std::stringstream output;
  EXPECT_NO_THROW(serializer.serialize(output));
  std::string fileContent = output.str();

  LOG << output.str();

  clearDb();

  serializer.deserialize(output);
  expectReadTestSuiteArticles();
  expectReadTestSuiteContainers();
  expectItemInContainer();

  clearDb();

  std::stringstream outputForSecondRead(fileContent);
  serializer.deserialize(outputForSecondRead);
  expectReadTestSuiteArticles();
  expectReadTestSuiteContainers();
  expectItemInContainer();
  expectItemInConsumedItems();
}