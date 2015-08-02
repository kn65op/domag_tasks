#pragma once

#include <iostream>
#include <map>
#include <stdexcept>
#include "Article.h"
#include "Container.h"
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
  using YamlNodes = std::vector<YAML::Node>;
  void storeVersion(std::ostream& output);
  template <typename AllEntitiesType> void serializeAllEntities(std::ostream& out, const AllEntitiesType & all_entities, const std::string& node_name);
  void storeArticlesId();
  void storeArticleAndItsDependentsId(const Article::ArticlePtr & article);
  void storeContainersId();
  void storeContainerAndItsDependentsId(const Container::ContainerPtr & container);
  YAML::Node serializeOwnData(const Article::ArticlePtr & article);
  YAML::Node serializeOwnData(const Container::ContainerPtr &container);

  auto getDependentEntities(const Article::ArticlePtr& article) -> decltype(article->getArticles())
  {
    LOG << "Getting articles for article: " << article->getName();
    return article->getArticles();
  }

  auto getDependentEntities(const Container::ContainerPtr& container) -> decltype(container->getContainers())
  {
    LOG << "Getting articles for article: " << container->getName();
    return container->getContainers();
  }

  int getDependetEntityId(const Article::ArticlePtr& article)
  {
    LOG << "Getting article id for : " << article->getName();
    return serializationArticles[article];
  }

  int getDependetEntityId(const Container::ContainerPtr& container)
  {
    LOG << "Getting article id for : " << container->getName();
    return serializationContainers[container];
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
        LOG << "serialize entity: " << node;
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
    LOG << "serialize entity data: " << node;
    return node;
  }

  void loadAndCheckVersion(const YAML::Node &main_node);
  void checkAndDeserializeAllArticles(const YAML::Node &database);
  std::map<int, YAML::Node> deserializeAllArticles(const YAML::Node &articles);
  void createArticles(std::map<int, YAML::Node> &&articles);
  void createDependentArticles(Article::ArticlePtr &article, const YAML::Node &article_node, std::map<int, YAML::Node> &all_articles);
  void checkAndDeserializeAllContainers(const YAML::Node &database);
  std::map<int, YAML::Node> deserializeAllContainers(const YAML::Node &containers);
  void createContainers(std::map<int, YAML::Node> &&containers);
  void createDependentContainers(Container::ContainerPtr &container, const YAML::Node &container_node, std::map<int, YAML::Node> &all_containers);

  std::map<Article::ArticlePtr, int> serializationArticles;
  std::map<Container::ContainerPtr, int> serializationContainers;
  std::string version_field = "Version";
  const int version_suported = 1;
};

}
}
