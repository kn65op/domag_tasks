#pragma once

#include <iostream>
#include <map>
#include "Article.h"
#include "yaml-cpp/yaml.h"

namespace depot
{
namespace serialize
{

class DepotSerializer
{
public:
  void serialize(std::ostream& out);

private:
  using YamlNodes = std::vector<YAML::Node>;
  void serializeAllArticles(std::ostream& out);

  template <typename Entity> YamlNodes serializeEntity(const std::shared_ptr<Entity>& entity)
  {
    YamlNodes nodes;
    nodes.push_back(std::move(serializeEntityData(article)));
    for (const auto & dependent_entity : getDependentEntities(article))
    {
      for (auto & node : serializeArticle(dependent_entity))
      {
        nodes.push_back(std::move(node));
      }
    }
    return nodes;
  }

  void storeArticlesId();
  void storeArticleAndItsDependentsId(const Article::ArticlePtr & article);
  YAML::Node serializeOwnData(const Article::ArticlePtr & article);
  auto getDependentEntities(const Article::ArticlePtr& article) -> decltype(article->getArticles())
  {
    return article->getArticles();
  }
  int getDependetEntityId(const Article::ArticlePtr& article)
  {
    return articles[article];
  }


  template<typename Entity> YAML::Node serializeEntityData(const std::shared_ptr<Entity>& entity)
  {
    auto node = serializeOwnData(entity);
    for (const auto & dependent_entity : getDependentEntities(entity))
    {
      node["dependent_articles"].push_back(getDependetEntityId(dependent_entity));
    }
    return node;
  }

  std::map<Article::ArticlePtr, int> articles;
};

}
}
