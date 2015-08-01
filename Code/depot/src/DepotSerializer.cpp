#include "../inc/DepotSerializer.h"
#include "../inc/Article.h"
#include "yaml-cpp/yaml.h"
#include "iostream"

using namespace depot::serialize;

void DepotSerializer::serialize(std::ostream& out)
{
  storeVersion(out);
  storeArticlesId();
  serializeAllArticles(out);
}

void DepotSerializer::storeVersion(std::ostream& out)
{
  YAML::Node version_node;
  version_node[version_field] = version_suported;
  out << version_node << "\n";
}

void DepotSerializer::loadAndCheckVersion(const YAML::Node& main_node)
{
  if (!main_node[version_field])
  {
    throw InvalidVersion();
  }
  const auto version = main_node[version_field].as<int>();
  if (version != version_suported)
  {
    throw InvalidVersion(version);
  }
}


void DepotSerializer::deserialize(std::istream& input)
{
  auto database = YAML::Load(input);
  loadAndCheckVersion(database);
  checkAndDeserializeAllArticles(database);
}

void DepotSerializer::checkAndDeserializeAllArticles(const YAML::Node& database)
{
  const auto articles = database["Articles"];
  if (articles)
  {
    createArticles(deserializeAllArticles(articles));
  }
}

auto DepotSerializer::deserializeAllArticles(const YAML::Node& articles) -> std::map<int, YAML::Node>
{
  std::map<int, YAML::Node> arts;
  for (const auto &article : articles)

  {
    arts.emplace(article["id"].as<int>(), article);
  }
  return arts;
}

void DepotSerializer::createArticles(std::map<int, YAML::Node> &&articles)
{
  while (!articles.empty())
  {
    const auto &article_node = articles.begin()->second;
    auto actual_article = Article::createTopLevelArticle(article_node["name"].as<std::string>(), article_node["unit"].as<std::string>());
    createDependentArticles(actual_article, article_node, articles);
    articles.erase(articles.begin());
  }
}

void DepotSerializer::createDependentArticles(Article::ArticlePtr &article, const YAML::Node &article_node, std::map<int, YAML::Node> &all_articles)
{
  if (article_node["dependent_articles"])
  {
    for (const auto & dependent_article_id : article_node["dependent_articles"])
    {
      const auto & dependent_article_node = all_articles.at(dependent_article_id.as<int>());
      auto dependent_article = article->createDependentArticle(dependent_article_node["name"].as< std::string>(), dependent_article_node["unit"].as<std::string>());
      createDependentArticles(dependent_article, dependent_article_node, all_articles);
      all_articles.erase(dependent_article_id.as<int>());
    }
  }
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
  article_node["id"] = serializationArticles[article];
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
  LOG << "store article id: " << article->getName() << " = " << serializationArticles.size() + 1;
  serializationArticles[article] = serializationArticles.size();
  for (const auto & dependent_article : article->getArticles())
  {
    storeArticleAndItsDependentsId(dependent_article);
  }
}
