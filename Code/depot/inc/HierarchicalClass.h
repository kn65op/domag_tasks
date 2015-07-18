#pragma once

#include <memory>
#include <vector>
#include <algorithm>

namespace depot
{

template <typename Entity> class HierarchicalClass
{
public:
//  using NoPrecedentException = typename Entity::NoPrecedentException;
//  using CircularDependencyException = typename Entity::CircularDependencyException;
//  using NoInfreriorException = typename Entity::NoInferiorException;

  using EntitySharedPtr = std::shared_ptr<Entity>;
  using EntityWeakPtr = std::weak_ptr<Entity>;

  using EntitiesContainer = std::vector<EntitySharedPtr>;

  template<typename ...Args> static EntitySharedPtr createTopLevelEntity(Args ... args)
  {
    Entity::doCreationChecks(args...);
    EntitySharedPtr new_entity{new Entity(args...)};
    addEntityToTopLevelEntities(new_entity);
    return new_entity;
  }

  static void removeTopLevelEntity(EntitySharedPtr entity)
  {
    auto entity_position = std::find(top_level_entities.begin(), top_level_entities.end(), entity);
    if (entity_position != top_level_entities.end())
    {
      top_level_entities.erase(entity_position);
    }
  }

  static const EntitiesContainer& getTopLevelEntities()
  {
    return top_level_entities;
  }

  static void clearTopLevelEntites()
  {
    top_level_entities.clear();
  }

  static void addEntityToTopLevelEntities(EntitySharedPtr entity)
  {
    top_level_entities.push_back(entity);
  }

  static EntitiesContainer top_level_entities;
};

}