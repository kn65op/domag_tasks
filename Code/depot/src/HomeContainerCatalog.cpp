#include "HomeContainerCatalog.h"

#include "Container.h"

#include <TLogger.h>

namespace depot
{
using ContainerHierarchy = HierarchicalClass<Container>;

void HomeContainerCatalog::clearTopLevelContainers()
{
  LOG << "Clearing top level containers";
  ContainerHierarchy::clearTopLevelEntites();
}

const HomeContainerCatalog::Containers& HomeContainerCatalog::getTopLevelContainers() const
{
  LOG << "Getting top level containers";
  return ContainerHierarchy::getTopLevelEntities();
}

HomeContainerCatalog::ContainerInside HomeContainerCatalog::createTopLevelContainer(const std::string& name)
{
  LOG << "Creating top level container: " << name;
  return ContainerHierarchy::createTopLevelEntity(name);
}

void HomeContainerCatalog::removeTopLevelContainer(ContainerInside container)
{
  LOG << "Creating top level container: " << container->getName();
  return ContainerHierarchy::removeTopLevelEntity(container);

}

}
