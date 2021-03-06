#pragma once

#include <iostream>
#include <map>
#include <stdexcept>
#include "Article.hpp"
#include "Container.hpp"
#include "yaml-cpp/yaml.h"
#include <TLogger.h>

namespace depot
{
namespace serialize
{

class DepotSerializer
{
public:
  struct InvalidVersion : std::logic_error
  {
    InvalidVersion() : std::logic_error("Unknown Version of file")
    {
    }

    InvalidVersion(int version) : std::logic_error("Input version is not supported: " + std::to_string(version))
    {
    }
  };
  void serialize(std::ostream& output);
  void deserialize(std::istream& input);

private:
  using AbstractContainer = depot::AbstractContainer;
  using YamlNodes = std::vector<YAML::Node>;
  void storeVersion(std::ostream& output);
  template <typename AllEntitiesType> void serializeAllEntities(std::ostream& out, const AllEntitiesType & all_entities, const std::string& node_name);
  void storeEntityAndItsDependentsId(const Article::DependentArticle & article);
  void storeEntityAndItsDependentsId(const Container::ContainerInside & container);
  YAML::Node serializeOwnData(const Article::DependentArticle & article);
  YAML::Node serializeOwnData(const Container::ContainerInside &container);

  template <typename TopLevelEntities> void storeEntitiesId(const TopLevelEntities & top_level_entities)
  {
    for (const auto & entity : top_level_entities)
    {
      storeEntityAndItsDependentsId(entity);
    }
  }

  auto getDependentEntities(const Article::DependentArticle& article) -> decltype(article->getArticles())
  {
    return article->getArticles();
  }

  auto getDependentEntities(const Container::ContainerInside& container) -> decltype(container->getContainers())
  {
    return container->getContainers();
  }

  int getDependetEntityId(const Article::DependentArticle& article)
  {
    return serializationArticles[article];
  }

  int getDependetEntityId(const Container::ContainerInside& container)
  {
    return serializationContainers[container.get()];
  }

  template <typename Entity> YamlNodes serializeEntity(const std::shared_ptr<Entity>& entity)
  {
    LOG << "serialize entity";
    YamlNodes nodes;
    nodes.push_back(std::move(serializeEntityData(entity)));
    for (const auto & dependent_entity : getDependentEntities(entity))
    {
      for (auto & node : serializeEntity(dependent_entity))
      {
        nodes.push_back(std::move(node));
      }
    }
    return nodes;
  }

  template<typename Entity> YAML::Node serializeEntityData(const std::shared_ptr<Entity>& entity)
  {
    LOG << "serialize entity data";
    auto node = serializeOwnData(entity);
    LOG << node;
    for (const auto & dependent_entity : getDependentEntities(entity))
    {
      node["dependents"].push_back(getDependetEntityId(dependent_entity));
    }
    return node;
  }

  template<typename TopLevelEntities> void storeEntities(std::ostream& out, const TopLevelEntities & entities, const std::string& entitesName)
  {
    storeEntitiesId(entities);
    serializeAllEntities(out, entities, entitesName);
  }

  void loadAndCheckVersion(const YAML::Node &main_node);
  void checkAndDeserializeAllArticles(const YAML::Node &database);
  void checkAndDeserializeAllItems(const YAML::Node &database);
  std::map<int, YAML::Node> deserializeEntitiesById(const YAML::Node &articles);
  void createArticles(std::map<int, YAML::Node> &&articles);
  void createDependentArticles(Article::DependentArticle &article, const YAML::Node &article_node, std::map<int, YAML::Node> &all_articles);
  void checkAndDeserializeAllContainers(const YAML::Node &database);
  void createContainers(std::map<int, YAML::Node> &&containers);
  void createDependentContainers(Container::BaseContainer container, const YAML::Node &container_node, std::map<int, YAML::Node> &all_containers);
  void storeItems(std::ostream & out, const Container::Containers & containers, const ItemsContainer &);
  std::vector<YAML::Node> getNodesForItemsInContainerAndSubcontainers(const HierarchicalItemsContainer&);
  YAML::Node storeItem(const depot::IItem * item);
  void cleanupSerialization();
  void cleanupDeserialization();
  void storeTrashContainer(const ItemsContainer &);
  void loadTrashContainer(AbstractContainer &);

  std::map<std::shared_ptr<IArticle>, int> serializationArticles;
  std::map<const AbstractContainer*, int> serializationContainers;
  std::map<int, std::shared_ptr<IArticle>> deserializationArticles;
  std::map<int, AbstractContainer*> deserializationContainers;
  std::string version_field = "Version";
  const int version_suported = 1;
  const std::string articlesName = "Articles";
  const std::string containersName = "Containers";
  const std::string consumedContainerName = "consumed";
  const std::string itemsName = "Items";
};

}
}
