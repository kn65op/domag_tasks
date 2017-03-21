#include "Container.h"
#include <algorithm>
#include <TLogger.h>

using namespace depot;

template<typename Entity> typename depot::HierarchicalClass<Entity>::EntitiesContainer depot::HierarchicalClass<Entity>::top_level_entities;


std::weak_ptr<AbstractContainer> Container::getStorehauseImpl() const
{
  return getPrecedentEntity();
}

std::string Container::getName() const
{
  return name;
}

std::shared_ptr<AbstractContainer> Container::getSharedPointer()
{
  return shared_from_this();
}