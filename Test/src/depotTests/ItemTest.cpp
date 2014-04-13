#include "gtest/gtest.h"
#include "depot/inc/Item.h"
#include "ThingMock.h"

using namespace ::testing;
using depot::Item;
using depot::IItem;

//TODO: best before
//TODO: buy and consume

struct ItemTest : public Test
{
  std::unique_ptr<Item> item;
  std::shared_ptr<depot::IThing> thing_mock{nullptr};
  
  void SetUp()
  {
    thing_mock = std::make_shared<depot::ut::ThingMock>();
    item = std::unique_ptr<Item>{new Item{thing_mock}};
  }
  
  void TearDown()
  {
    thing_mock.reset();
    
    //clear expectations
  }
    
};

TEST_F(ItemTest, ItemCanBeBuyedWithoutPrice)
{
  item->buy(3);
  EXPECT_EQ(item->getQuantity(), 3);
}

TEST_F(ItemTest, ItemShouldBeBuyedOnlyOnce)
{
  item->buy(1);
  ASSERT_THROW(item->buy(1), IItem::ItemAlreadyBuyed);
}

TEST_F(ItemTest, ItemBuyedWithPriceShouldKnowPricePerUnit)
{
  item->buy(3, 6.30);
  ASSERT_EQ(item->getQuantity(), 3);
  EXPECT_EQ(item->getPricePerUnit(), 2.10);
}

TEST_F(ItemTest, ShouldNotBeAbleToConsumeWhenThereIsNoItem)
{       
  ASSERT_THROW(item->consume(0.01), IItem::NoQuantityToConsume);
}

TEST_F(ItemTest, ShouldNotBeAbleToConsumeMoreThenIsAvailable)
{
  item->buy(1);
  ASSERT_THROW(item->consume(1.01), IItem::NoQuantityToConsume);
}

TEST_F(ItemTest, AfterConsumptionQuantityShouldDecrease)
{
  item->buy(1);
  item->consume(0.5);
  ASSERT_EQ(item->getQuantity(), 0.5);
}

TEST_F(ItemTest, BuyByDefaultShouldSetTodayAsBuyDate)
{
  item->buy(1);
  ASSERT_EQ(item->getBuyDate(), boost::gregorian::day_clock::local_day());
}

TEST_F(ItemTest, BuyShouldSetSpecifiedBuyDate)
{
  std::string date = "2013-01-01";
  item->buy(1, 0, boost::gregorian::from_simple_string(date));
  ASSERT_EQ(item->getBuyDate(), boost::gregorian::from_simple_string(date));
}