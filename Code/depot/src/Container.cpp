#include "Container.h"
#include <TLogger.h>
#include <algorithm>

using namespace depot;

template <typename Entity, typename EntityInterface>
typename depot::HierarchicalClass<Entity, EntityInterface>::EntitiesContainer
    depot::HierarchicalClass<Entity, EntityInterface>::top_level_entities;

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
