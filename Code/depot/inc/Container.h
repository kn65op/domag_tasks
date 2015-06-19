#pragma once

#include <memory>
#include <functional>
#include "AbstractContainer.h"
#include "Storable.h"
#include "Item.h"

namespace depot
{

class Container : public AbstractContainer, public Storable
{
public:
  using Item = std::unique_ptr<IItem>;
  using Items = std::vector<Item>;
  using ItemReference = std::reference_wrapper<Item>;
  using SelectedItems = std::vector<ItemReference>;
  using ContainerInside = std::shared_ptr<Container>;
  using Containers = std::vector<ContainerInside> ;

  virtual ~Container() {}

  void addItem(std::unique_ptr<IItem> item);
  Item removeItem(const Item & to_remove);
  Item removeItem(ItemReference to_remove);
  void addContainer(ContainerInside container);
  ContainerInside removeContainer(ContainerInside container);
  const Items & getItems() const;
  const Containers& getContainers() const;
  const SelectedItems getNonConsumedItems();
private:
  std::string name;
  Items items;
  Containers containers;

  void removeFromContainer() override;
  void addToContainer(AbstractContainer& new_container) override;
};

}
