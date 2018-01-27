#pragma once

#include "HierarchicalClass.h"
#include "ItemsContainer.h"
#include "Storable.h"
#include <TLogger.h>
#include <functional>

namespace depot
{

class Container : public std::enable_shared_from_this<Container>,
                  public HierarchicalItemsContainer,
                  public Storable,
                  HierarchicalClass<Container>,
                  public ItemsContainer
{
  public:
    using ItemsContainer::addItem;
    using ItemsContainer::removeItem;
    using ItemsContainer::getItems;
    using ItemsContainer::getNonConsumedItems;

    struct LiesNowhere : public std::logic_error
    {
        LiesNowhere(const std::string& msg = "Container is not inside other container") : std::logic_error(msg)
        {
        }
    };

    struct CannotInsertContainerIntoItself : public std::logic_error
    {
        CannotInsertContainerIntoItself(const std::string& msg = "Container cannot  be inside itself")
            : std::logic_error(msg)
        {
        }
    };

    struct InvalidContainer : public std::logic_error
    {
        InvalidContainer(const std::string& msg = "pointer is not container") : std::logic_error(msg)
        {
        }
    };

    Container(const Container&) = delete;
    Container* operator=(const Container&) = delete;

    ~Container() override
    {
        LOG << "Delete Container: " << name;
    }

    using Item = depot::IItem::Ptr;
    using ItemPtr = depot::IItem*;
    using ContainerInside = std::shared_ptr<Container>;
    using BaseContainer = std::shared_ptr<HierarchicalItemsContainer>;
    using ContainerPtr = std::shared_ptr<Container>;
    using Containers = std::vector<ContainerInside>;

    using NoPrecedentException = LiesNowhere;
    using CircularDependencyException = CannotInsertContainerIntoItself;
    using NoInferiorException = AbstractContainer::NoSuchElement;

    std::string getName() const override;

    void addContainer(std::shared_ptr<HierarchicalItemsContainer> container) override
    {
        addInferiorEntity(std::dynamic_pointer_cast<Container>(container));
    }

    BaseContainer removeContainer(BaseContainer container) override
    {
        return removeInferiorEntity(std::dynamic_pointer_cast<Container>(container));
    }

    const Containers& getContainers() const
    {
        return getInferiorEntities();
    }

    std::shared_ptr<HierarchicalItemsContainer> createDependentContainer() override
    {
        return createDependentContainerImpl("Unnamed container");
    }

    std::shared_ptr<HierarchicalItemsContainer> createDependentContainer(const std::string& name) override
    {
        return createDependentContainerImpl(name);
    }

    std::shared_ptr<Container> makeSharedPointer() override
    {
        return shared_from_this();
    }

  private:
    friend class HierarchicalClass<Container>;
    Container(const std::string& name_arg) : name{name_arg}
    {
        LOG << "Create Container: " << name;
    }

    static void doCreationChecks(const std::string&)
    {
        LOG << "For now no checks done";
    }

    std::shared_ptr<Container> createDependentContainerImpl(const std::string& name)
    {
        return createDependentEntity(name);
    }

    std::weak_ptr<AbstractContainer> getStorehauseImpl() const override;
    std::shared_ptr<AbstractContainer> getSharedPointer() override;

    std::string name;
};
}
