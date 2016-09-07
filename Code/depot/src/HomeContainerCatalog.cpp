#include "HomeContainerCatalog.h"

#include "Container.h"
#include "ConsumedItems.h"

#include <TLogger.h>

namespace depot
{
using ContainerHierarchy = HierarchicalClass<Container>;

std::shared_ptr<depot::ConsumedItems> HomeContainerCatalog::consumedItems = std::make_shared<depot::ConsumedItems>();

void HomeContainerCatalog::clearAllContainers()
{
  LOG << "Clearing top level containers";
  ContainerHierarchy::clearTopLevelEntites();
  LOG << "Clearing consumed items";
  consumedItems = std::make_shared<depot::ConsumedItems>();
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

depot::ItemsContainer& HomeContainerCatalog::getContainerForConsumedItems() const
{
  return *consumedItems;
}

}
