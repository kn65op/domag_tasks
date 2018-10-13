#include "gtest/gtest.h"

#include "depot/inc/Container.hpp"
#include "depot/inc/HomeContainerCatalog.hpp"
#include "ItemMock.hpp"

using namespace ::testing;
using depot::Container;
using depot::ut::ItemMock;

struct ContainerTest : public Test
{
  depot::HomeContainerCatalog catalog;
  std::shared_ptr<Container> c = createContainer();

  ~ContainerTest()
  {
    catalog.clearAllContainers();
  }

  std::shared_ptr<Container> createContainer()
  {
      return std::dynamic_pointer_cast<Container>(catalog.createTopLevelContainer());
  }

  void expectMoveItemBetweenContainers(ItemMock& item)
  {
    EXPECT_CALL(item, setStorehause(_)).Times(2);
  }

  void expectMoveItemToContainer(ItemMock& item)
  {
    EXPECT_CALL(item, setStorehause(_)).Times(1);
  }

  void expectRemoveItemFromContainer(ItemMock& item)
  {
    EXPECT_CALL(item, setStorehause(_)).Times(1);
  }

  ItemMock* expectAddItemToContainer(std::unique_ptr<ItemMock> &&item)
  {
    auto mock = item.get();
    expectMoveItemToContainer(*mock);
    c->addItem(std::move(item));
    return mock;
  }

  auto makeStrictItemMock() -> decltype(std::make_unique<StrictMock<ItemMock>>())
  {
    return std::make_unique<StrictMock<ItemMock>>();
  }

  ItemMock* expectAddItemToContainer()
  {
    auto item = makeStrictItemMock();
    return expectAddItemToContainer(std::move(item));
  }

  ItemMock* expectAddItemWithQuantityToContainer(int quanity, int times = 1)
  {
    auto item = makeStrictItemMock();
    EXPECT_CALL(*item, getQuantity()).Times(times).WillRepeatedly(Return(quanity));
    return expectAddItemToContainer(std::move(item));
  }
};

TEST_F(ContainerTest, CanAddItemToContainer)
{
  expectAddItemToContainer();
}

TEST_F(ContainerTest, GetItemsShouldReturnOneItemAfterOneAdded)
{
  expectAddItemToContainer();

  const auto items = c->getItems();

  EXPECT_EQ(1U, items.size());
}

TEST_F(ContainerTest, GetItemsShouldReturnAllItemsAfterSomeAdded)
{
  expectAddItemToContainer();
  expectAddItemToContainer();
  expectAddItemToContainer();
  expectAddItemToContainer();

  EXPECT_EQ(4u, c->getItems().size());
}

TEST_F(ContainerTest, GetNotConsumedShouldReturnOnlyNotConsumedItems)
{
  expectAddItemWithQuantityToContainer(1);
  expectAddItemWithQuantityToContainer(0);
  expectAddItemWithQuantityToContainer(5);
  expectAddItemWithQuantityToContainer(5);

  EXPECT_EQ(3u, c->getNonConsumedItems().size());
}

TEST_F(ContainerTest, RemoveItemShouldWorkForConsumedItems)
{
  auto qantity_expected = 1;
  expectAddItemWithQuantityToContainer(0);
  auto item = expectAddItemWithQuantityToContainer(qantity_expected, 2);

  ASSERT_EQ(2u, c->getItems().size());

  expectRemoveItemFromContainer(*item);
  auto removed_item = c->removeItem(c->getNonConsumedItems()[0]);
  EXPECT_EQ(qantity_expected, removed_item->getQuantity());

  EXPECT_EQ(1u, c->getItems().size());
}

TEST_F(ContainerTest, ContainerShouldHaveValidSizeAfterRemoveOneItem)
{
  expectAddItemToContainer();
  auto item = expectAddItemToContainer();

  ASSERT_EQ(2u, c->getItems().size());

  expectRemoveItemFromContainer(*item);
  c->removeItem(c->getItems()[1]);

  EXPECT_EQ(1u, c->getItems().size());
}

TEST_F(ContainerTest, RemoveItemShouldThrowWhenRemovingNonExistingItem)
{
  StrictMock<ItemMock> item;
  EXPECT_THROW(c->removeItem(&item), Container::NoSuchElement);
}

TEST_F(ContainerTest, RemoveContainerShouldThrowWhenRemovingNonExistingConstainer)
{
  EXPECT_THROW(c->removeContainer(createContainer()), Container::NoSuchElement);
}

TEST_F(ContainerTest, ShouldKnowThatItNotBelogsToOtherContainer)
{
  ASSERT_FALSE(c->getStorehause());
}

TEST_F(ContainerTest, AfterAddingOneContainerShouldContainerInsideShouldKnowWhereItLiesAndAfterRemovalItShouldHaveNone)
{
  auto container = createContainer();
  auto container_inside = createContainer();
  container->addContainer(container_inside);
  EXPECT_EQ(container, container_inside->getStorehause().value());
  container->removeContainer(container_inside);
  ASSERT_FALSE(container_inside->getStorehause());
}

TEST_F(ContainerTest, AfterAddingOneContainerShouldReturnOneContainerAndShouldBeRemoveable)
{
  auto cont = c->createDependentContainer();
  EXPECT_EQ(1U, c->getContainers().size());
  c->removeContainer(cont);
  EXPECT_EQ(0U, c->getContainers().size());
}

TEST_F(ContainerTest, ContainerShouldNotCreateCircularDpendecies)
{
  auto container_inside = c->createDependentContainer();
  auto container_bottom = container_inside->createDependentContainer();
  EXPECT_THROW(container_inside->addContainer(c), Container::CannotInsertContainerIntoItself);
  EXPECT_THROW(container_bottom->addContainer(c), Container::CannotInsertContainerIntoItself);
}

TEST_F(ContainerTest, AfterMoveContainerToOtherContainerFirstShouldNotHaveItAndSecondShouldHave)
{
  auto container_second = createContainer();
  auto container_inside = createContainer();

  c->addContainer(container_inside);
  ASSERT_EQ(1U, c->getContainers().size());
  ASSERT_EQ(0U, container_second->getContainers().size());

  container_second->addContainer(c->removeContainer(container_inside));

  EXPECT_EQ(0U, c->getContainers().size());
  EXPECT_EQ(1U, container_second->getContainers().size());
}

TEST_F(ContainerTest, ItemShouldBeMovedFromOneContainerToAnother)
{
  auto container_second = createContainer();
  auto itemMock = expectAddItemToContainer();

  ASSERT_EQ(1U, c->getItems().size());
  ASSERT_EQ(0U, container_second->getItems().size());

  expectMoveItemBetweenContainers(*itemMock);

  container_second->addItem(c->removeItem(c->getItems()[0]));

  EXPECT_EQ(0U, c->getItems().size());
  EXPECT_EQ(1U, container_second->getItems().size());
}
