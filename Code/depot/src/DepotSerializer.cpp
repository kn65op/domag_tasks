#include "../inc/DepotSerializer.h"
#include "../inc/Article.h"
#include "yaml-cpp/yaml.h"

using namespace depot::serialize;

void DepotSerializer::serialize(std::ostream& out)
{
  out << "Version: 1\n";
  storeArticlesId();
  serializeAllArticles(out);
}

void DepotSerializer::serializeAllArticles(std::ostream& out)
{
  YAML::Node all_articles_node;
  const auto top_level_articles = depot::Article::getTopLevelArticles();
  for (const auto & article : top_level_articles)
  {
    for (auto & node : serializeEntity(article))
    {
      all_articles_node["Articles"].push_back(std::move(node));
    }
  }
  out << all_articles_node;
}

YAML::Node DepotSerializer::serializeOwnData(const Article::ArticlePtr& article)
{
  LOG << "serialize article data: " << article->getName();
  YAML::Node article_node;
  LOG << "serialize article data: " << article_node;
  article_node["id"] = articles[article];
  LOG << "serialize article data: " << article_node;
  article_node["name"] = article->getName();
  LOG << "serialize article data: " << article_node;
  article_node["unit"] = article->getUnit();
  LOG << "serialize article data: " << article_node;
  return article_node;
}

void DepotSerializer::storeArticlesId()
{
  const auto top_level_articles = depot::Article::getTopLevelArticles();
  for (const auto & article : top_level_articles)
  {
    storeArticleAndItsDependentsId(article);
  }
}

void DepotSerializer::storeArticleAndItsDependentsId(const Article::ArticlePtr & article)
{
  LOG << "store article id: " << article->getName() << " = " << articles.size() + 1;
  articles[article] = articles.size();
  for (const auto & dependent_article : article->getArticles())
  {
    storeArticleAndItsDependentsId(dependent_article);
  }
}
