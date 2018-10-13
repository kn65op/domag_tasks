#pragma once

#include "HierarchicalClass.hpp"
#include "ItemsContainer.hpp"
#include "Storable.hpp"
#include <TLogger.h>
#include <functional>

namespace depot
{

class Container : public std::enable_shared_from_this<Container>,
                  public HierarchicalItemsContainer,
                  public Storable,
                  private ItemsContainer,
                  HierarchicalClass<Container, HierarchicalItemsContainer>
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
    using BaseContainer = std::shared_ptr<HierarchicalItemsContainer>;
    using ContainerPtr = std::shared_ptr<Container>;

    using NoPrecedentException = LiesNowhere;
    using CircularDependencyException = CannotInsertContainerIntoItself;
    using NoInferiorException = AbstractContainer::NoSuchElement;

    std::string getName() const override;

    void addContainer(std::shared_ptr<HierarchicalItemsContainer> container) override
    {
        addInferiorEntity(std::dynamic_pointer_cast<Container>(container), this);
    }

    BaseContainer removeContainer(BaseContainer container) override
    {
        return removeInferiorEntity(std::dynamic_pointer_cast<Container>(container));
    }

    const Containers& getContainers() const override
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
    friend class HierarchicalClass<Container, HierarchicalItemsContainer>;
    Container(const std::string& name_arg) : name{name_arg}
    {
        LOG << "Create Container: " << name;
    }

    static void doCreationChecks(const std::string&)
    {
        LOG << "For now no checks done";
    }

    std::shared_ptr<HierarchicalItemsContainer> createDependentContainerImpl(const std::string& name)
    {
        return createDependentEntity(name);
    }

    std::optional<std::shared_ptr<AbstractContainer>> getStorehauseImpl() const override;
    std::shared_ptr<AbstractContainer> getSharedPointer() override;

    std::string name;
};
}
