#pragma once

#include <memory>
#include <functional>
#include "HierarchicalClass.h"
#include "AbstractContainer.h"
#include "Storable.h"
#include "Item.h"

namespace depot
{

class Container : public std::enable_shared_from_this<Container>, public AbstractContainer, public Storable, public HierarchicalClass<Container>
{
public:
  struct LiesNowhere : public std::logic_error
  {
    LiesNowhere(const std::string& msg = "Container is not inside other container") :
      std::logic_error(msg)
    {}
  };

  struct CannotInsertContainerIntoItself : public std::logic_error
  {
    CannotInsertContainerIntoItself(const std::string& msg = "Container cannot  be inside itself") :
      std::logic_error(msg)
    {}
  };

  struct InvalidContainer : public std::logic_error
  {
    InvalidContainer(const std::string& msg = "pointer is not container") :
      std::logic_error(msg)
    {}
  };

  Container(const Container &) = delete;
  Container* operator=(const Container&) = delete;

  using Item = depot::IItem::Ptr;
  using Items = std::vector<Item>;
  using ItemReference = depot::IItem::Reference;
  using SelectedItems = std::vector<ItemReference>;
  using ContainerInside = std::shared_ptr<Container>;
  using Containers = std::vector<ContainerInside> ;

  using NoPrecedentException = LiesNowhere;
  using CircularDependencyException = CannotInsertContainerIntoItself;
  using NoInferiorException = AbstractContainer::NoSuchElement;

  virtual ~Container() {}

  void addItem(std::unique_ptr<IItem> item);
  Item removeItem(const Item & to_remove);
  Item removeItem(const ItemReference to_remove);
  const Items & getItems() const;
  const SelectedItems getNonConsumedItems();

  void addContainer(ContainerInside container)
  {
    addInferiorEntity(container);
  }

  ContainerInside removeContainer(ContainerInside container)
  {
    return removeInferiorEntity(container);
  }

  Containers& getContainers()
  {
    return getInferiorEntities();
  }

  static std::shared_ptr<Container> createTopLevelContainer()
  {
    return createTopLevelEntity();
  }

  static std::shared_ptr<Container> makeSharedPtr(HierarchicalClass<Container>* container_candidate)
  {
    auto container = dynamic_cast<Container*>(container_candidate);
    if (container)
    {
      return container->shared_from_this();
    }
    throw InvalidContainer();
  }

  static void doCreationChecks()
  {
  }

  static void clearTopLevelContainers()
  {
    clearTopLevelEntites();
  }

//  static void create
private:
  friend class HierarchicalClass<Container> ;
  Container() = default;

  std::string name;
  Items items;

  std::shared_ptr<AbstractContainer> getStorehauseImpl() const override;
};

}
