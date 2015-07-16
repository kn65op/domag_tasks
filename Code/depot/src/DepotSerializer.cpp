#include "../inc/DepotSerializer.h"
#include "../inc/Article.h"
#include "yaml-cpp/yaml.h"

using namespace depot::serialize;

void DepotSerializer::serialize(std::ostream& out)
{
  out << "Version: 1\n";
  auto top_level_articles = depot::TopLevelArticles::getTopLevelArticles();
  YAML::Node all_articles_node;
  for (auto & article : top_level_articles)
  {
    YAML::Node article_node;
    article_node["name"] = article->getName();
    article_node["unit"] = article->getUnit();
    all_articles_node["Articles"].push_back(article_node);
  }
  out << all_articles_node;
}
