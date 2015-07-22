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
  YamlNodes serializeArticle(const Article::ArticlePtr & article);
  void storeArticlesId();
  void storeArticleAndItsDependentsId(const Article::ArticlePtr & article);
  YAML::Node serializeOwnData(const Article::ArticlePtr & article);
  auto getDependentEntities(const Article::ArticlePtr& article) -> typeof(article->getArticles())
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
    for (const auto & dependent_entity : getDependentEntities(entity)->getArticles())
    {
      article_node["dependent_articles"].push_back(getDependetEntityId(dependent_entity));
    }
    return article_node;
  }

  std::map<Article::ArticlePtr, int> articles;
};

}
}
