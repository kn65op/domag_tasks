#pragma once

#include "AbstractContainer.h"

namespace depot
{

class ItemsContainer : virtual public AbstractContainer
{
  public:
    void addItem(std::unique_ptr<IItem> item) override;
    IItem::Ptr removeItem(const IItem* to_remove) override;
    const SelectedItems getItems() const override;
    const SelectedItems getNonConsumedItems() const override;

  private:
    using Items = std::vector<IItem::Ptr>;

    virtual std::shared_ptr<AbstractContainer> getSharedPointer() = 0;

    Items items;
};

class HierarchicalContainer : virtual public AbstractContainer
{
public:
    virtual std::shared_ptr<HierarchicalContainer> createDependentContainer(const std::string&) = 0;
    virtual std::shared_ptr<HierarchicalContainer> createDependentContainer() = 0;
    virtual void addContainer(std::shared_ptr<HierarchicalContainer>) = 0;
    virtual std::shared_ptr<HierarchicalContainer> removeContainer(std::shared_ptr<HierarchicalContainer>) = 0;
};

class HierarchicalItemsContainer  : public HierarchicalContainer, public ItemsContainer
{
public:
    using ContainerInside = std::shared_ptr<HierarchicalItemsContainer >;
    using Containers = std::vector<ContainerInside>;

    virtual const Containers& getContainers() const = 0;
};

}
