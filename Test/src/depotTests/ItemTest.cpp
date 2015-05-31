#include "gtest/gtest.h"
#include "depot/inc/Item.h"

#include "../../inc/depotMocks/ArticleMock.h"

using namespace ::testing;
using depot::Item;
using depot::IItem;

//TODO: best before
//TODO: buy and consume

struct ItemTest : public Test
{
  using Date = boost::gregorian::date;

  std::shared_ptr<depot::ut::ArticleMock> thing = std::make_shared<depot::ut::ArticleMock>();
  Item item;

  ItemTest() :
    item{thing}
  {}

  void SetUp()
  {
  }

  void TearDown()
  {
  }

};

TEST_F(ItemTest, ItemCanBeBuyedWithoutPrice)
{
  item.buy(3);
  EXPECT_EQ(item.getQuantity(), 3);
}

TEST_F(ItemTest, ItemShouldBeBuyedOnlyOnce)
{
  item.buy(1);
  ASSERT_THROW(item.buy(1), IItem::ItemAlreadyBuyed);
}

TEST_F(ItemTest, ItemBuyedWithPriceShouldKnowPricePerUnit)
{
  item.buy(3, 6.30);
  ASSERT_EQ(item.getQuantity(), 3);
  EXPECT_EQ(item.getPricePerUnit(), 2.10);
}

TEST_F(ItemTest, ShouldNotBeAbleToConsumeWhenThereIsNoItem)
{
  ASSERT_THROW(item.consume(0.01), IItem::NoQuantityToConsume);
}

TEST_F(ItemTest, ShouldNotBeAbleToConsumeMoreThenIsAvailable)
{
  item.buy(1);
  ASSERT_THROW(item.consume(1.01), IItem::NoQuantityToConsume);
}

TEST_F(ItemTest, AfterConsumptionQuantityShouldDecrease)
{
  item.buy(1);
  item.consume(0.5);
  ASSERT_EQ(item.getQuantity(), 0.5);
}

TEST_F(ItemTest, BuyByDefaultShouldSetTodayAsBuyDate)
{
  item.buy(1);
  ASSERT_EQ(item.getBuyDate(), boost::gregorian::day_clock::local_day());
}

TEST_F(ItemTest, BuyShouldSetSpecifiedBuyDate)
{
  std::string date = "2013-01-01";
  item.buy(1, 0, boost::gregorian::from_simple_string(date));
  ASSERT_EQ(item.getBuyDate(), boost::gregorian::from_simple_string(date));
}

TEST_F(ItemTest, ConsumeShouldStoreItsHistory)
{
  item.buy(1);
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
  item.buy(1);
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
  EXPECT_EQ(name, item.getThing()->getName());
}
