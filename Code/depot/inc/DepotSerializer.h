#pragma once

#include <iostream>
#include <map>
#include <stdexcept>
#include "Article.h"
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
  void serializeAllArticles(std::ostream& out);
  void storeArticlesId();
  void storeArticleAndItsDependentsId(const Article::ArticlePtr & article);
  YAML::Node serializeOwnData(const Article::ArticlePtr & article);

  auto getDependentEntities(const Article::ArticlePtr& article) -> decltype(article->getArticles())
  {
    LOG << "Getting articles for article: " << article->getName();
    return article->getArticles();
  }
  int getDependetEntityId(const Article::ArticlePtr& article)
  {
    LOG << "Getting article id for : " << article->getName();
    return articles[article];
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
      node["dependent_articles"].push_back(getDependetEntityId(dependent_entity));
    }
    LOG << "serialize entity data: " << node;
    return node;
  }

  void loadAndCheckVersion(const YAML::Node &main_node);

  std::map<Article::ArticlePtr, int> articles;
  std::string version_field = "Version";
  const int version_suported = 1;
};

}
}
