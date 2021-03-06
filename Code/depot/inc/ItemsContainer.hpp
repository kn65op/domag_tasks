#pragma once

#include <optional>

#include "AbstractContainer.hpp"

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

class HierarchicalItemsContainer : virtual public AbstractContainer
{
public:
    using ContainerInside = std::shared_ptr<HierarchicalItemsContainer >;
    using Containers = std::vector<ContainerInside>;

    virtual std::shared_ptr<HierarchicalItemsContainer> createDependentContainer(const std::string&) = 0;
    virtual std::shared_ptr<HierarchicalItemsContainer> createDependentContainer() = 0;
    virtual void addContainer(std::shared_ptr<HierarchicalItemsContainer>) = 0;
    virtual std::shared_ptr<HierarchicalItemsContainer> removeContainer(std::shared_ptr<HierarchicalItemsContainer>) = 0;
    virtual const Containers& getContainers() const = 0;
    virtual std::optional<std::shared_ptr<HierarchicalItemsContainer>> getPrecedent() const = 0;
};

}
