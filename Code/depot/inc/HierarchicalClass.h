#pragma once

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <vector>

#include "TLogger.h"

namespace depot
{

template <typename Entity, typename EntityInterface = Entity> class HierarchicalClass
{
  public:
    struct NoPrecedentException : public std::logic_error
    {
      public:
        NoPrecedentException(const std::string& msg = "Cannot set empty name") : std::logic_error(msg)
        {
        }
    };

    struct CircularDependencyException : public std::logic_error
    {
      public:
        CircularDependencyException(const std::string& msg = "Cannot set empty name") : std::logic_error(msg)
        {
        }
    };

    struct NoInferiorException : public std::logic_error
    {
      public:
        NoInferiorException(const std::string& msg = "Cannot set empty name") : std::logic_error(msg)
        {
        }
    };

    using RawEntitySharedPtr = std::shared_ptr<Entity>;
    using EntitySharedPtr = std::shared_ptr<EntityInterface>;
    using EntityWeakPtr = std::weak_ptr<EntityInterface>;

    using EntitiesContainer = std::vector<EntitySharedPtr>;
    using InferiorEntitiesContainer = std::vector<EntitySharedPtr>;

    virtual ~HierarchicalClass() = default;

    template <typename... Args> static EntitySharedPtr createTopLevelEntity(Args... args)
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

  protected:
    template <typename... Args> EntitySharedPtr createDependentEntity(Args... args)
    {
        Entity::doCreationChecks(args...);
        EntitySharedPtr new_entity{new Entity(args...)};
        addInferiorEntity(new_entity);
        return new_entity;
    }

    void addInferiorEntity(EntitySharedPtr entity, const Entity* this_object)
    {
        try
        {
            checkIfNotMakingCircularDependency(entity, this_object);
        }
        catch (const CircularDependencyException& original_exception)
        {
            throw typename Entity::CircularDependencyException(original_exception.what());
        }
        addInferiorEntity(entity);
    }

    void addInferiorEntity(EntitySharedPtr entity)
    {
        inferior_entities.push_back(entity);
        std::dynamic_pointer_cast<Entity>(entity)->precedent = makeSharedPointer();
        removeTopLevelEntity(entity);
    }

  void checkIfNotMakingCircularDependency(EntitySharedPtr entity, const Entity* this_object) const
    {
        if (entity.get() == dynamic_cast<const EntityInterface*>(this_object))
        {
            throw CircularDependencyException("Trying to make entity self inferior");
        }
        try
        {
            if (auto precedent_real = precedent.lock())
            {
                const auto precedent_real_casted = std::dynamic_pointer_cast<Entity>(precedent_real);
                precedent_real_casted->checkIfNotMakingCircularDependency(entity, precedent_real_casted.get());
            }
        }
        catch (const CircularDependencyException&)
        {
            throw CircularDependencyException("Trying to make entity circular dependent");
        }
    }

    const InferiorEntitiesContainer& getInferiorEntities() const
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
        std::dynamic_pointer_cast<Entity>(*removed_entity)->precedent.reset();
        addEntityToTopLevelEntities(*removed_entity);
        return *removed_entity;
    }

    EntitySharedPtr getPrecedentEntity() const
    {
        if (auto precedent_real = precedent.lock())
        {
            return precedent_real;
        }
        throw typename Entity::NoPrecedentException();
    }

    // private
    static void addEntityToTopLevelEntities(EntitySharedPtr entity)
    {
        top_level_entities.push_back(entity);
    }

    virtual RawEntitySharedPtr makeSharedPointer() = 0;

  private:
    static EntitiesContainer top_level_entities;

    EntityWeakPtr precedent;
    InferiorEntitiesContainer inferior_entities;
};
}
