#pragma once

#include <memory>
#include <functional>
#include "AbstractContainer.h"
#include "Storable.h"
#include "Item.h"

namespace depot
{

class Container : public std::enable_shared_from_this<Container>, public AbstractContainer, public Storable
{
public:
  struct LiesNowhere
  {

  };
  Container(const Container &) = delete;
  Container* operator=(const Container&) = delete;

  using Item = std::unique_ptr<IItem>;
  using Items = std::vector<Item>;
  using ItemReference = std::reference_wrapper<Item>;
  using SelectedItems = std::vector<ItemReference>;
  using ContainerInside = std::shared_ptr<Container>;
  using Containers = std::vector<ContainerInside> ;

  static std::shared_ptr<Container> createContainer()
  {
    return std::shared_ptr<Container>(new Container());
  }
  virtual ~Container() {}

  void addItem(std::unique_ptr<IItem> item);
  Item removeItem(const Item & to_remove);
  Item removeItem(const ItemReference to_remove);
  void addContainer(ContainerInside container);
  ContainerInside removeContainer(ContainerInside container);
  const Items & getItems() const;
  const Containers& getContainers() const;
  const SelectedItems getNonConsumedItems();
private:
  Container() = default;

  std::string name;
  Items items;
  Containers containers;
  std::shared_ptr<Container> storehause;

  void removeFromContainer() override;
  void addToContainer(std::shared_ptr<AbstractContainer> new_container) override;
  std::shared_ptr<AbstractContainer> getStorehauseImpl() const override;
};

}
