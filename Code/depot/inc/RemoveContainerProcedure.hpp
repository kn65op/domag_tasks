#pragma once

#include <stdexcept>
#include <memory>

namespace depot
{
class ContainerCatalog;
class HierarchicalItemsContainer;

class RemoveContainerProcedure
{
public:
    struct ContainerNotFound : public std::logic_error
    {
        using std::logic_error::logic_error;
    };

    virtual ~RemoveContainerProcedure() noexcept = default;

    virtual void removeContainer(std::shared_ptr<HierarchicalItemsContainer>) = 0;
};

class RemoveAnyContainerProcedure : public RemoveContainerProcedure
{
public:
    RemoveAnyContainerProcedure(std::shared_ptr<ContainerCatalog>);

    void removeContainer(std::shared_ptr<HierarchicalItemsContainer>) override;

private:
    std::shared_ptr<ContainerCatalog> catalog;
};

}
