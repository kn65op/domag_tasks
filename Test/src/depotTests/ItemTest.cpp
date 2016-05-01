#include "gtest/gtest.h"
#include "depot/inc/Item.h"

#include "../../inc/depotMocks/ArticleMock.h"
#include "depot/inc/Container.h"

using namespace ::testing;
using depot::Item;
using depot::IItem;

//TODO: best before

struct ItemTest : public Test
{
  using Date = boost::gregorian::date;

  std::shared_ptr<depot::ut::ArticleMock> thing = std::make_shared<depot::ut::ArticleMock>();
  const double initialAmount{1.0};
  Item item{thing, {initialAmount}};
};

TEST_F(ItemTest, ItemCanBeBuyedWithoutPrice)
{
  constexpr auto amount = 3.38;

  Item item(thing, {amount});
  EXPECT_EQ(amount, item.getQuantity());
  EXPECT_EQ(amount, item.getBoughtAmount());
}

TEST_F(ItemTest, ItemBuyedWithPriceShouldKnowPricePerUnit)
{
  Item item(thing, {3.0, 6.30});
  ASSERT_EQ(item.getQuantity(), 3.0);
  EXPECT_EQ(item.getPricePerUnit(), 2.10);
}

TEST_F(ItemTest, ShouldNotBeAbleToConsumeWhenThereIsNoItem)
{
  Item item(thing, {0.0});
  ASSERT_THROW(item.consume(0.01), IItem::NoQuantityToConsume);
}

TEST_F(ItemTest, ShouldNotBeAbleToConsumeMoreThenIsAvailable)
{
  ASSERT_THROW(item.consume(1.01), IItem::NoQuantityToConsume);
}

TEST_F(ItemTest, AfterConsumptionQuantityShouldDecreaseAndBoughtAmountShouldNot)
{
  constexpr auto amount = 3.88;
  constexpr auto consume = 0.5;
  constexpr auto rest = 3.38;

  Item item(thing, {amount});
  item.consume(consume);

  EXPECT_EQ(rest, item.getQuantity());
  EXPECT_EQ(amount, item.getBoughtAmount());
}

TEST_F(ItemTest, BuyByDefaultShouldSetTodayAsBuyDate)
{
  ASSERT_EQ(item.getBuyDate(), boost::gregorian::day_clock::local_day());
}

TEST_F(ItemTest, BuyShouldSetSpecifiedBuyDate)
{
  const std::string date = "2013-01-01";

  Item item(thing, {1.0, 0.0, boost::gregorian::from_simple_string(date)});
  ASSERT_EQ(item.getBuyDate(), boost::gregorian::from_simple_string(date));
}

TEST_F(ItemTest, ConsumeShouldStoreItsHistory)
{
  item.consume(0.1);
  item.consume(0.5);
  item.consume(0.4);

  depot::ConsumeHistory::List list;
  Date now = boost::gregorian::day_clock::local_day();

  list.push_back({0.1, now});
  list.push_back({0.5, now});
  list.push_back({0.4, now});

  ASSERT_EQ(item.getQuantity(), 0);

  EXPECT_EQ(item.getConsumeHistory(), list);
}

TEST_F(ItemTest, ConsumeShouldStoreItsHistoryWithDates)
{
  std::string sdate = "2013-01-01";
  item.consume(0.1, boost::gregorian::from_simple_string(sdate));
  item.consume(0.5);
  item.consume(0.4);

  depot::ConsumeHistory::List list;
  Date now = boost::gregorian::day_clock::local_day();

  list.push_back({0.1, boost::gregorian::from_simple_string(sdate)});
  list.push_back({0.5, now});
  list.push_back({0.4, now});

  ASSERT_EQ(item.getQuantity(), 0);

  EXPECT_EQ(item.getConsumeHistory(), list);
}

TEST_F(ItemTest, GetNameOfItemShouldReturnValidName)
{
  std::string name{"name"};
  EXPECT_CALL(*thing, getNameMock()).WillOnce(Return(name));
  EXPECT_EQ(name, item.getThing().lock()->getName());
}

TEST_F(ItemTest, AfterSetStorehauseShouldHaveItAndAfterRemovalShouldNotHave)
{
  using depot::Container;
  EXPECT_THROW(item.getStorehause(), Item::NoStorehause);
  auto container = Container::createTopLevelContainer();
  item.setStorehause(container);
  EXPECT_EQ(container, item.getStorehause().lock());
  item.setStorehause(std::shared_ptr<depot::AbstractContainer>(nullptr));
  EXPECT_THROW(item.getStorehause(), Item::NoStorehause);
  Container::clearTopLevelContainers();
}

TEST_F(ItemTest, ShouldNotAcceptEmptyArticle)
{
  EXPECT_THROW(Item(std::shared_ptr<depot::IArticle>(nullptr), {1.0}), Item::ArticleCannotBeEmpty);

  EXPECT_THROW(item.changeArticle(std::shared_ptr<depot::IArticle>(nullptr)), Item::ArticleCannotBeEmpty);
}

TEST_F(ItemTest, ShouldBeAbleToChangeArticle)
{
  std::string name{"name"};
  EXPECT_CALL(*thing, getNameMock()).WillOnce(Return(name));
  EXPECT_EQ(name, item.getThing().lock()->getName());

  auto second_thing = std::make_shared<depot::ut::ArticleMock>();
  std::string second_name{"second name"};
  item.changeArticle(second_thing);
  EXPECT_CALL(*second_thing, getNameMock()).WillOnce(Return(second_name));
  EXPECT_EQ(second_name, item.getThing().lock()->getName());
}
