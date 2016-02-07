#include "DepotSerializer.h"
#include "yaml-cpp/yaml.h"
#include "iostream"

using namespace depot::serialize;

void DepotSerializer::serialize(std::ostream& out)
{
  storeVersion(out);
  storeEntitiesId(Article::getTopLevelArticles());
  serializeAllEntities(out, Article::getTopLevelArticles(), "Articles");
  storeEntitiesId(Container::getTopLevelContainers());
  serializeAllEntities(out, Container::getTopLevelContainers(), "Containers");
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
  checkAndDeserializeAllContainers(database);
}

void DepotSerializer::checkAndDeserializeAllArticles(const YAML::Node& database)
{
  const auto articles = database["Articles"];
  if (articles)
  {
    createArticles(deserializeEntitiesById(articles));
  }
}

void DepotSerializer::checkAndDeserializeAllContainers(const YAML::Node& database)
{
  const auto containers = database["Containers"];
  if (containers)
  {
    createContainers(deserializeEntitiesById(containers));
  }
}

auto DepotSerializer::deserializeEntitiesById(const YAML::Node& all_nodes) -> std::map<int, YAML::Node>
{
  std::map<int, YAML::Node> nodes;
  for (const auto &node : all_nodes)
  {
    nodes.emplace(node["id"].as<int>(), node);
  }
  return nodes;
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

void DepotSerializer::createContainers(std::map<int, YAML::Node> &&containers)
{
  while (!containers.empty())
  {
    const auto &container_node = containers.begin()->second;
    auto actual_container = Container::createTopLevelContainer(container_node["name"].as<std::string>());
    createDependentContainers(actual_container, container_node, containers);
    containers.erase(containers.begin());
  }
}

void DepotSerializer::createDependentArticles(Article::ArticlePtr &article, const YAML::Node &article_node, std::map<int, YAML::Node> &all_articles)
{
  if (article_node["dependents"])
  {
    for (const auto & dependent_article_id : article_node["dependents"])
    {
      const auto & dependent_article_node = all_articles.at(dependent_article_id.as<int>());
      auto dependent_article = article->createDependentArticle(dependent_article_node["name"].as< std::string>(), dependent_article_node["unit"].as<std::string>());
      createDependentArticles(dependent_article, dependent_article_node, all_articles);
      all_articles.erase(dependent_article_id.as<int>());
    }
  }
}

void DepotSerializer::createDependentContainers(Container::ContainerPtr &container, const YAML::Node &container_node, std::map<int, YAML::Node> &all_containers)
{
  if (container_node["dependents"])
  {
    for (const auto & dependent_container_id : container_node["dependents"])
    {
      const auto & dependent_container_node = all_containers.at(dependent_container_id.as<int>());
      auto dependent_container = container->createDependentContainer(dependent_container_node["name"].as< std::string>());
      createDependentContainers(dependent_container, dependent_container_node, all_containers);
      all_containers.erase(dependent_container_id.as<int>());
    }
  }
}

template <typename AllEntitiesType> void DepotSerializer::serializeAllEntities(std::ostream& out, const AllEntitiesType & all_entities, const std::string& node_name)
{
  YAML::Node all_entities_node;
  for (const auto & article : all_entities)
  {
    for (auto & node : serializeEntity(article))
    {
      all_entities_node[node_name].push_back(std::move(node));
    }
  }
  out << all_entities_node;
}

YAML::Node DepotSerializer::serializeOwnData(const Article::ArticlePtr& article)
{
  LOG << "serialize article data: " << article->getName();
  YAML::Node article_node;
  article_node["id"] = serializationArticles[article];
  article_node["name"] = article->getName();
  article_node["unit"] = article->getUnit();
  return article_node;
}

YAML::Node DepotSerializer::serializeOwnData(const Container::ContainerPtr &container)
{
  YAML::Node container_node;
  container_node["id"] = serializationContainers[container];
  container_node["name"] = container->getName();
  return container_node;
}

void DepotSerializer::storeEntityAndItsDependentsId(const Article::ArticlePtr & article)
{
  LOG << "store article id: " << article->getName() << " = " << serializationArticles.size() + 1;
  serializationArticles[article] = serializationArticles.size();
  for (const auto & dependent_article : article->getArticles())
  {
    storeEntityAndItsDependentsId(dependent_article);
  }
}

void DepotSerializer::storeEntityAndItsDependentsId(const Container::ContainerPtr & container)
{
  LOG << "store container id: " << container->getName() << " = " << serializationContainers.size() + 1;
  serializationContainers[container] = serializationContainers.size();
  for (const auto & dependent_container : container->getContainers())
  {
    storeEntityAndItsDependentsId(dependent_container);
  }
}
