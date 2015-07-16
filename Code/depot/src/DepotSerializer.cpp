#include "../inc/DepotSerializer.h"
#include "../inc/Article.h"
#include "yaml-cpp/yaml.h"

using namespace depot::serialize;

void DepotSerializer::serialize(std::ostream& out)
{
  out << "Version: 1\n";
  const auto top_level_articles = depot::TopLevelArticles::getTopLevelArticles();
  YAML::Node all_articles_node;
  for (const auto & article : top_level_articles)
  {
    for (auto & node : serializeArticle(article))
    {
      all_articles_node["Articles"].push_back(std::move(node));
    }
  }
  out << all_articles_node;
}

auto DepotSerializer::serializeArticle(const Article::ArticlePtr &article) -> YamlNodes
{
  YamlNodes nodes;
  YAML::Node article_node;
  article_node["name"] = article->getName();
  article_node["unit"] = article->getUnit();
  nodes.push_back(std::move(article_node));
  for (const auto & dependent_article : article->getArticles())
  {
    for (auto & node : serializeArticle(dependent_article))
    {
      nodes.push_back(std::move(node));
    }
  }
  return std::move(nodes);
}
