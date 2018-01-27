#include "depot/inc/AddNewContainerProcedure.hpp"
#include "depot/inc/Container.h"
#include "depot/inc/ContainerCatalog.h"

namespace depot
{

AddTopLevelContainerProcedure::AddTopLevelContainerProcedure(std::shared_ptr<depot::ContainerCatalog> cat)
    : catalog{std::move(cat)}
{
}

void AddTopLevelContainerProcedure::add(const std::string& name)
{
    catalog->createTopLevelContainer(name);
}

AddDependentContainerProcedure::AddDependentContainerProcedure(std::shared_ptr<depot::HierarchicalItemsContainer> c)
    : container{std::move(c)}
{
}

void AddDependentContainerProcedure::add(const std::string& name)
{
    container->createDependentContainer(name);
}
}
