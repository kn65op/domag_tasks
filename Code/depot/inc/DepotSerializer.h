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
  YAML::Node serializeArticleData(const Article::ArticlePtr & article);
  void storeArticlesId();
  void storeArticleAndItsDependentsId(const Article::ArticlePtr & article);

  std::map<Article::ArticlePtr, int> articles;
};

}
}
