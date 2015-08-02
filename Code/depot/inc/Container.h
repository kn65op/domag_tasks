#pragma once

#include <memory>
#include <functional>
#include "HierarchicalClass.h"
#include "AbstractContainer.h"
#include "Storable.h"
#include "Item.h"
#include <TLogger.h>

namespace depot
{

class Container : public std::enable_shared_from_this<Container>, public AbstractContainer, public Storable, public HierarchicalClass<
    Container>
{
public:
  struct LiesNowhere : public std::logic_error
  {
    LiesNowhere(const std::string& msg = "Container is not inside other container") :
        std::logic_error(msg)
    {
    }
  };

  struct CannotInsertContainerIntoItself : public std::logic_error
  {
    CannotInsertContainerIntoItself(const std::string& msg = "Container cannot  be inside itself") :
        std::logic_error(msg)
    {
    }
  };

  struct InvalidContainer : public std::logic_error
  {
    InvalidContainer(const std::string& msg = "pointer is not container") :
        std::logic_error(msg)
    {
    }
  };

  Container(const Container &) = delete;
  Container* operator=(const Container&) = delete;

  using Item = depot::IItem::Ptr;
  using Items = std::vector<Item>;
  using ItemReference = depot::IItem::Reference;
  using SelectedItems = std::vector<ItemReference>;
  using ContainerInside = std::shared_ptr<Container>;
  using ContainerPtr = std::shared_ptr<Container>;
  using Containers = std::vector<ContainerInside>;

  using NoPrecedentException = LiesNowhere;
  using CircularDependencyException = CannotInsertContainerIntoItself;
  using NoInferiorException = AbstractContainer::NoSuchElement;

  virtual ~Container()
  {
    LOG << "Delete Container: " << name;
  }

  void addItem(std::unique_ptr<IItem> item);
  Item removeItem(const Item & to_remove);
  Item removeItem(const ItemReference to_remove);
  const Items & getItems() const;
  const SelectedItems getNonConsumedItems();
  std::string getName() const;

  void addContainer(ContainerInside container)
  {
    addInferiorEntity(container);
  }

  ContainerInside removeContainer(ContainerInside container)
  {
    return removeInferiorEntity(container);
  }

  const Containers& getContainers() const
  {
    return getInferiorEntities();
  }

  static std::shared_ptr<Container> createTopLevelContainer(const std::string& name = "Unnamed container")
  {
    return createTopLevelEntity(name);
  }

  static void removeTopLevelContainer(std::shared_ptr<Container> container)
  {
    removeTopLevelEntity(container);
  }

  static const Containers& getTopLevelContainers()
  {
    return getTopLevelEntities();
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

  static void clearTopLevelContainers()
  {
    clearTopLevelEntites();
  }

  std::shared_ptr<Container> createDependentContainer(const std::string &name = "Unnamed container")
  {
    return createDependentEntity(name);
  }

private:
  friend class HierarchicalClass<Container> ;
  Container(const std::string& name_arg) :
    name{name_arg}
  {
    LOG << "Create Container: " << name;
  }

  static void doCreationChecks(const std::string&)
  {
    LOG << "For now no checks done";
  }

  std::shared_ptr<AbstractContainer> getStorehauseImpl() const override;

  std::string name;
  Items items;
};

}
