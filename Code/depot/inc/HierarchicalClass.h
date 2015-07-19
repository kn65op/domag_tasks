#pragma once

#include <memory>
#include <vector>
#include <algorithm>
#include <stdexcept>

namespace depot
{

template<typename Entity> class HierarchicalClass
{
public:
  struct NoPrecedentException : public std::logic_error
  {
  public:
    NoPrecedentException(const std::string& msg = "Cannot set empty name") : std::logic_error(msg)
    {}
  };

  struct CircularDependencyException : public std::logic_error
  {
  public:
    CircularDependencyException(const std::string& msg = "Cannot set empty name") : std::logic_error(msg)
    {}
  };

  struct NoInferiorException : public std::logic_error
  {  public:
    NoInferiorException(const std::string& msg = "Cannot set empty name") : std::logic_error(msg)
    {}
  };

  using EntitySharedPtr = std::shared_ptr<Entity>;
  using EntityWeakPtr = std::weak_ptr<Entity>;

  using EntitiesContainer = std::vector<EntitySharedPtr>;
  using InferiorEntitiesContainer = std::vector<EntitySharedPtr>;

  virtual ~HierarchicalClass()
  {
  }

protected:
  template<typename ...Args> static EntitySharedPtr createDependentEntity(EntitySharedPtr precedent, Args ... args)
  {
    Entity::doCreationChecks(args...);
    EntitySharedPtr new_entity {new Entity(args...)};
    precedent->addInferiorEntity(new_entity);
    return new_entity;
  }

  void addInferiorEntity(EntitySharedPtr entity)
  {
    checkIfNotMakingCircularDependency(entity);
    inferior_entities.push_back(entity);
    entity->precedent = Entity::makeSharedPtr(this);
    removeTopLevelEntity(entity);
  }

  void checkIfNotMakingCircularDependency(EntitySharedPtr entity)
  try
  {
    if (entity.get() == this)
    {
      throw CircularDependencyException("Trying to make entity self inferior");
    }
    try
    {
      if (auto precedent_real = precedent.lock())
      {
        precedent_real->checkIfNotMakingCircularDependency(entity);
      }
    }
    catch (const CircularDependencyException&)
    {
      throw CircularDependencyException("Trying to make entity circular dependent");
    }
  }
  catch (const CircularDependencyException& original_exception)
  {
    throw typename Entity::CircularDependencyException(original_exception.what());
  }

  InferiorEntitiesContainer& getInferiorEntities()
  {
    return inferior_entities;
  }

  EntitySharedPtr removeInferiorEntity(EntitySharedPtr entity)
  {
    const auto entity_position = std::find(inferior_entities.begin(), inferior_entities.end(), entity);
    if (entity_position == inferior_entities.end())
    {
      throw typename Entity::NoInferiorException();
    }
    auto removed_entity = inferior_entities.erase(entity_position);
    (*removed_entity)->precedent.reset();
    addEntityToTopLevelEntities(*removed_entity);
    return *removed_entity;
  }

  EntitySharedPtr getPrecedentEntity()
  {
    if (auto precedent_real = precedent.lock())
    {
      return precedent_real;
    }
    throw typename Entity::NoPrecedentException();
  }

  template<typename ...Args> static EntitySharedPtr createTopLevelEntity(Args ... args)
  {
    Entity::doCreationChecks(args...);
    EntitySharedPtr new_entity {new Entity(args...)};
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

private:
  static EntitiesContainer top_level_entities;

  EntityWeakPtr precedent;
  InferiorEntitiesContainer inferior_entities;

};

}
