#include "depot/inc/RemoveContainerProcedure.hpp"

#include "depot/inc/Container.hpp"
#include "depot/inc/ContainerCatalog.hpp"

namespace depot
{

RemoveAnyContainerProcedure::RemoveAnyContainerProcedure(std::shared_ptr<depot::ContainerCatalog> cat)
    : catalog{std::move(cat)}
{
}

void RemoveAnyContainerProcedure::removeContainer(std::shared_ptr<HierarchicalItemsContainer> container)
{
    try
    {
        if(!container)
        {
            throw ContainerNotFound{"Not able to remove nullptr"};
        }
        if (auto parentContainer = container->getPrecedent())
        {
            parentContainer.value()->removeContainer(container);
        }
        else
        {
            catalog->removeTopLevelContainer(container);
        }
    }
    catch (const ContainerCatalog::ContainerNotFound& ex)
    {
        throw ContainerNotFound{ex.what()};
    }
}
} // namespace depot
