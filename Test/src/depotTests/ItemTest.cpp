#include "gtest/gtest.h"
#include "depot/inc/Item.h"
#include "ThingMock.h"

using namespace ::testing;
using depot::Item;

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

TEST_F(ItemTest, itemNeedToBeKindOfSomeThing)
{
}