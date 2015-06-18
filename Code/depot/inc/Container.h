#pragma once

#include "Item.h"
#include <memory>
#include <functional>

namespace depot
{

class Container
{
public:
  struct NoSuchElement
  {
  };
  typedef std::unique_ptr<IItem> Item;
  typedef std::vector<Item> Items;
  typedef std::vector<std::reference_wrapper<IItem>> SelectedItems;
  using ContainerInside = std::shared_ptr<Container>;
  using Containers = std::vector<ContainerInside> ;

  void addItem(std::unique_ptr<IItem> item);
  Item removeItem(const Item & to_remove);
  void addContainer(ContainerInside container);
  void removeContainer(ContainerInside container);
  const Items & getItems() const;
  const SelectedItems getNonConsumedItems() const;
private:
  std::string name;
  Items items;
};

}
