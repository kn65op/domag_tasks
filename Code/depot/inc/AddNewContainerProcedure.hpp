#pragma once

#include <memory>
#include <string>

namespace depot
{
class ContainerCatalog;
class HierarchicalItemsContainer;

class AddContainerProcedure
{
  public:
    virtual ~AddContainerProcedure() noexcept = default;
    virtual std::weak_ptr<HierarchicalItemsContainer> add(const std::string& name) = 0;
};

class AddTopLevelContainerProcedure : public AddContainerProcedure
{
  public:
    AddTopLevelContainerProcedure(std::shared_ptr<ContainerCatalog>);

    std::weak_ptr<HierarchicalItemsContainer> add(const std::string&) override;

  private:
    std::shared_ptr<ContainerCatalog> catalog;
};

class AddDependentContainerProcedure : public AddContainerProcedure
{
  public:
    AddDependentContainerProcedure(std::shared_ptr<depot::HierarchicalItemsContainer>);

    std::weak_ptr<HierarchicalItemsContainer> add(const std::string&) override;

  private:
    std::shared_ptr<depot::HierarchicalItemsContainer> container;
};
}
