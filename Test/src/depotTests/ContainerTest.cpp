#include "gtest/gtest.h"
#include "depot/inc/Container.h"
#include "ItemMock.h"

using namespace ::testing;
using depot::Container;
using depot::ut::ItemMock;

struct ContainerTest : public Test
{
  std::shared_ptr<Container> c = Container::createContainer();

  void TearDown() override
  {
    Container::clearTopLevelContainers();
  }
};

TEST_F(ContainerTest, CanAddItemToContainer)
{
  c->addItem(std::move(std::unique_ptr<ItemMock>(new ItemMock())));
}

TEST_F(ContainerTest, GetItemsShouldReturnOneItemAfterOneAdded)
{
  std::unique_ptr<ItemMock> item(new ItemMock());

  c->addItem(std::move(item));

  const Container::Items &items = c->getItems();

  EXPECT_EQ(1U, items.size());
}

TEST_F(ContainerTest, GetItemsShouldReturnAllItemsAfterSomeAdded)
{
  c->addItem(std::move(std::unique_ptr<ItemMock>(new ItemMock())));
  c->addItem(std::move(std::unique_ptr<ItemMock>(new ItemMock())));
  c->addItem(std::move(std::unique_ptr<ItemMock>(new ItemMock())));
  c->addItem(std::move(std::unique_ptr<ItemMock>(new ItemMock())));

  EXPECT_EQ(c->getItems().size(), 4);
}

TEST_F(ContainerTest, GetNotConsumedShouldReturnOnlyNotConsumedItems)
{
  auto item1 = std::make_unique<ItemMock>();
  EXPECT_CALL(*item1, getQuantity()).WillOnce(Return(0));
  auto item2 = std::make_unique<ItemMock>();
  EXPECT_CALL(*item2, getQuantity()).WillOnce(Return(1));
  auto item3 = std::make_unique<ItemMock>();
  EXPECT_CALL(*item3, getQuantity()).WillOnce(Return(0));
  auto item4 = std::make_unique<ItemMock>();
  EXPECT_CALL(*item4, getQuantity()).WillOnce(Return(5));
  auto item5 = std::make_unique<ItemMock>();
  EXPECT_CALL(*item5, getQuantity()).WillOnce(Return(5));

  c->addItem(std::move(item1));
  c->addItem(std::move(item2));
  c->addItem(std::move(item3));
  c->addItem(std::move(item4));
  c->addItem(std::move(item5));

  EXPECT_EQ(c->getNonConsumedItems().size(), 3);
}

TEST_F(ContainerTest, RemoveItemShouldWorkForConsumedItems)
{
  auto item1 = std::make_unique<ItemMock>();
  EXPECT_CALL(*item1, getQuantity()).WillRepeatedly(Return(0));
  auto item2 = std::make_unique<ItemMock>();
  EXPECT_CALL(*item2, getQuantity()).WillRepeatedly(Return(1));

  c->addItem(std::move(item1));
  c->addItem(std::move(item2));

  ASSERT_EQ(c->getItems().size(), 2);

  auto quntity_expected = c->getNonConsumedItems()[0].get()->getQuantity();
  auto removed_item = c->removeItem(c->getNonConsumedItems()[0]);
  EXPECT_EQ(quntity_expected, removed_item->getQuantity());

  EXPECT_EQ(c->getItems().size(), 1);
}

TEST_F(ContainerTest, ContainerShouldHaveValidSizeAfterRemoveOneItem)
{
  c->addItem(std::move(std::unique_ptr<ItemMock>(new ItemMock())));
  c->addItem(std::move(std::unique_ptr<ItemMock>(new ItemMock())));

  ASSERT_EQ(c->getItems().size(), 2);

  c->removeItem(c->getItems()[1]);

  EXPECT_EQ(c->getItems().size(), 1);
}

TEST_F(ContainerTest, RemoveItemShouldThrowWhenRemovingNonExistingItem)
{
  EXPECT_THROW(c->removeItem(std::unique_ptr<ItemMock>(new ItemMock())), Container::NoSuchElement);
}

TEST_F(ContainerTest, RemoveContainerShouldThrowWhenRemovingNonExistingConstainer)
{
  EXPECT_THROW(c->removeContainer(Container::createContainer()), Container::NoSuchElement);
}
TEST_F(ContainerTest, ShouldKnowThatItNotBelogsToOtherContainer)
{
  EXPECT_THROW(c->getStorehause(), Container::LiesNowhere);
}

TEST_F(ContainerTest, AfterAddingOneContainerShouldContainerInsideShouldKnowWhereItLiesAndAfterRemovalItShouldHaveNone)
{
  auto container = Container::createContainer();
  auto container_inside = Container::createContainer();
  container->addContainer(container_inside);
  EXPECT_EQ(container, container_inside->getStorehause());
  container->removeContainer(container_inside);
  EXPECT_THROW(container_inside->getStorehause(), Container::LiesNowhere);
}

TEST_F(ContainerTest, AfterAddingOneContainerShouldReturnOneContainerAndShouldBeRemoveable)
{
  auto cont = Container::createContainer();
  c->addContainer(cont);
  EXPECT_EQ(1U, c->getContainers().size());
  c->removeContainer(cont);
  EXPECT_EQ(0U, c->getContainers().size());
}

TEST_F(ContainerTest, AfterMoveContainerToOtherContainerFirstShouldNotHaveItAndSecondShouldHave)
{
  auto container_second = Container::createContainer();
  auto container_inside = Container::createContainer();

  c->addContainer(container_inside);
  ASSERT_EQ(1U, c->getContainers().size());
  ASSERT_EQ(0U, container_second->getContainers().size());

  container_second->addContainer(c->removeContainer(container_inside));

  EXPECT_EQ(0U, c->getContainers().size());
  EXPECT_EQ(1U, container_second->getContainers().size());
}

TEST_F(ContainerTest, ItemShouldBeMovedFromOneContainerToAnother)
{
  auto container_second = Container::createContainer();
  auto item = std::make_unique<ItemMock>();
  c->addItem(std::move(item));

  ASSERT_EQ(1U, c->getItems().size());
  ASSERT_EQ(0U, container_second->getItems().size());

  container_second->addItem(c->removeItem(c->getItems()[0]));

  EXPECT_EQ(0U, c->getItems().size());
  EXPECT_EQ(1U, container_second->getItems().size());
}
