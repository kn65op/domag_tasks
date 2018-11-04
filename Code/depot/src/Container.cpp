#include "Container.hpp"
#include <TLogger.h>
#include <algorithm>

using namespace depot;

template <typename Entity, typename EntityInterface>
typename depot::HierarchicalClass<Entity, EntityInterface>::EntitiesContainer
    depot::HierarchicalClass<Entity, EntityInterface>::top_level_entities;

std::optional<std::shared_ptr<AbstractContainer>> Container::getStorehauseImpl() const
{
    const auto entity = getPrecedentEntity();
    if (entity)
    {
        return *entity;
    }
    return {};
}

std::string Container::getName() const
{
    return name;
}

std::shared_ptr<AbstractContainer> Container::getSharedPointer()
{
    return shared_from_this();
}

std::optional<std::shared_ptr<HierarchicalItemsContainer>> Container::getPrecedent() const
{
    return getPrecedentEntity();
}