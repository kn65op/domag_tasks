#include "gtest/gtest.h"
#include "depot/inc/Container.h"
#include "ItemMock.h"

using namespace ::testing;
using depot::Container;
using depot::ut::ItemMock;

struct ContainerTest : public Test
{
  Container c;
};

TEST_F(ContainerTest, CanAddItemToContainer)
{
  c.addItem(std::move(std::unique_ptr<ItemMock>(new ItemMock())));
}

TEST_F(ContainerTest, GetItemsShouldReturnOneItemAfterOneAdded)
{
  std::unique_ptr<ItemMock> item(new ItemMock());
    
  c.addItem(std::move(item));
  
  const Container::Items &items = c.getItems();
  
  EXPECT_EQ(items.size(), 1);
}

TEST_F(ContainerTest, GetItemsShouldReturnAllItemsAfterSomeAdded)
{
  c.addItem(std::move(std::unique_ptr<ItemMock>(new ItemMock())));
  c.addItem(std::move(std::unique_ptr<ItemMock>(new ItemMock())));
  c.addItem(std::move(std::unique_ptr<ItemMock>(new ItemMock())));
  c.addItem(std::move(std::unique_ptr<ItemMock>(new ItemMock())));
  
  EXPECT_EQ(c.getItems().size(), 4);
}

TEST_F(ContainerTest, GetNotConsumedShouldReturnOnlyNotConsumedItems)
{
  c.addItem(std::move(std::unique_ptr<ItemMock>(new ItemMock())));
  c.addItem(std::move(std::unique_ptr<ItemMock>(new ItemMock())));
  c.addItem(std::move(std::unique_ptr<ItemMock>(new ItemMock())));
  c.addItem(std::move(std::unique_ptr<ItemMock>(new ItemMock())));
  c.addItem(std::move(std::unique_ptr<ItemMock>(new ItemMock())));
  
  const Container::Items &items = c.getItems();
  EXPECT_CALL(*(dynamic_cast<ItemMock*>(items[0].get())), getQuantity()).WillOnce(Return(0));
  EXPECT_CALL(*(dynamic_cast<ItemMock*>(items[1].get())), getQuantity()).WillOnce(Return(1));
  EXPECT_CALL(*(dynamic_cast<ItemMock*>(items[2].get())), getQuantity()).WillOnce(Return(0));
  EXPECT_CALL(*(dynamic_cast<ItemMock*>(items[3].get())), getQuantity()).WillOnce(Return(5));
  EXPECT_CALL(*(dynamic_cast<ItemMock*>(items[4].get())), getQuantity()).WillOnce(Return(5));
  
  EXPECT_EQ(c.getNonConsumedItems().size(), 3);
}

TEST_F(ContainerTest, ContainerShouldHaveValidSizeAfterRemoveOneItem)
{
  c.addItem(std::move(std::unique_ptr<ItemMock>(new ItemMock())));
  c.addItem(std::move(std::unique_ptr<ItemMock>(new ItemMock())));
  
  ASSERT_EQ(c.getItems().size(), 2);
  
  c.removeItem(c.getItems()[1]);
  
  EXPECT_EQ(c.getItems().size(), 1);
}

//TEST_F(ContainerTest, ItemShouldBeMovedFromOneContainerToAnother)