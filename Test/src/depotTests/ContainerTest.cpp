#include "gtest/gtest.h"
#include "depot/inc/Container.h"
#include "ItemMock.h"

using namespace ::testing;
using depot::Container;
using depot::ut::ItemMock;

struct ContainerTest : public Test
{
  std::shared_ptr<Container> c = Container::createContainer();
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

  EXPECT_EQ(items.size(), 1);
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
  c->addItem(std::move(std::unique_ptr<ItemMock>(new ItemMock())));
  c->addItem(std::move(std::unique_ptr<ItemMock>(new ItemMock())));
  c->addItem(std::move(std::unique_ptr<ItemMock>(new ItemMock())));
  c->addItem(std::move(std::unique_ptr<ItemMock>(new ItemMock())));
  c->addItem(std::move(std::unique_ptr<ItemMock>(new ItemMock())));

  const Container::Items &items = c->getItems();
  EXPECT_CALL(*(dynamic_cast<ItemMock*>(items[0].get())), getQuantity()).WillOnce(Return(0));
  EXPECT_CALL(*(dynamic_cast<ItemMock*>(items[1].get())), getQuantity()).WillOnce(Return(1));
  EXPECT_CALL(*(dynamic_cast<ItemMock*>(items[2].get())), getQuantity()).WillOnce(Return(0));
  EXPECT_CALL(*(dynamic_cast<ItemMock*>(items[3].get())), getQuantity()).WillOnce(Return(5));
  EXPECT_CALL(*(dynamic_cast<ItemMock*>(items[4].get())), getQuantity()).WillOnce(Return(5));

  EXPECT_EQ(c->getNonConsumedItems().size(), 3);
}

TEST_F(ContainerTest, RemoveItemShouldWorkForConsumedItems)
{
  c->addItem(std::move(std::unique_ptr<ItemMock>(new ItemMock())));
  c->addItem(std::move(std::unique_ptr<ItemMock>(new ItemMock())));

  const Container::Items &items = c->getItems();
  EXPECT_CALL(*(dynamic_cast<ItemMock*>(items[0].get())), getQuantity()).WillRepeatedly(Return(0));
  EXPECT_CALL(*(dynamic_cast<ItemMock*>(items[1].get())), getQuantity()).WillRepeatedly(Return(1));

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

TEST_F(ContainerTest, AfterAddingOneContainerShouldContainerInsideShouldKnowWhereItLies)
{
  auto container = Container::createContainer();
  auto container_inside = Container::createContainer();
  container->addContainer(container_inside);
  EXPECT_EQ(container, container_inside->getStorehause());
}

TEST_F(ContainerTest, AfterAddingOneContainerShouldReturnOneContainerAndShouldBeRemoveable)
{
  auto cont = Container::createContainer();
  c->addContainer(cont);
  EXPECT_EQ(1, c->getContainers().size());
  c->removeContainer(cont);
  EXPECT_EQ(0, c->getContainers().size());
}

TEST_F(ContainerTest, AfterMoveContainerToOtherContainerFirstShouldNotHaveItAndSecondShouldHave)
{
  auto container_second = Container::createContainer();
  auto container_inside = Container::createContainer();

  c->addContainer(container_inside);
  ASSERT_EQ(1, c->getContainers().size());
  ASSERT_EQ(0, container_second->getContainers().size());

  container_second->addContainer(c->removeContainer(container_inside));

  EXPECT_EQ(0, c->getContainers().size());
  EXPECT_EQ(1, container_second->getContainers().size());
}

//TEST_F(ContainerTest, ItemShouldBeMovedFromOneContainerToAnother)