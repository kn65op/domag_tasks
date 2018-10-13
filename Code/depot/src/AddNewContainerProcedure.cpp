#include "depot/inc/AddNewContainerProcedure.hpp"
#include "depot/inc/Container.hpp"
#include "depot/inc/ContainerCatalog.hpp"

namespace depot
{

AddTopLevelContainerProcedure::AddTopLevelContainerProcedure(std::shared_ptr<depot::ContainerCatalog> cat)
    : catalog{std::move(cat)}
{
}

std::weak_ptr<HierarchicalItemsContainer> AddTopLevelContainerProcedure::add(const std::string& name)
{
    return catalog->createTopLevelContainer(name);
}

AddDependentContainerProcedure::AddDependentContainerProcedure(std::shared_ptr<depot::HierarchicalItemsContainer> c)
    : container{std::move(c)}
{
}

std::weak_ptr<HierarchicalItemsContainer> AddDependentContainerProcedure::add(const std::string& name)
{
    return container->createDependentContainer(name);
}
}
