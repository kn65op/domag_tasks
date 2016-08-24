#pragma once

#include <functional>
#include "HierarchicalClass.h"
#include "ItemsContainer.h"
#include "Storable.h"
#include <TLogger.h>

namespace depot
{

class Container : public std::enable_shared_from_this<Container>, public ItemsContainer,
                  public Storable, HierarchicalClass< Container>
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

  ~Container() override
  {
    LOG << "Delete Container: " << name;
  }

  using Item = depot::IItem::Ptr;
  using ItemPtr = depot::IItem*;
  using ContainerInside = std::shared_ptr<Container>;
  using ContainerPtr = std::shared_ptr<Container>;
  using Containers = std::vector<ContainerInside>;

  using NoPrecedentException = LiesNowhere;
  using CircularDependencyException = CannotInsertContainerIntoItself;
  using NoInferiorException = AbstractContainer::NoSuchElement;

  std::string getName() const override;

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

  std::shared_ptr<Container> createDependentContainer(const std::string &name = "Unnamed container")
  {
    return createDependentEntity(name);
  }

  std::shared_ptr<Container> makeSharedPointer()
  {
    return shared_from_this();
  }

private:
  friend class HierarchicalClass<Container>;
  Container(const std::string& name_arg) :
    name{name_arg}
  {
    LOG << "Create Container: " << name;
  }

  static void doCreationChecks(const std::string&)
  {
    LOG << "For now no checks done";
  }

  std::weak_ptr<AbstractContainer> getStorehauseImpl() const override;
  std::shared_ptr<AbstractContainer> getSharedPointer() override;

  std::string name;
};

}
