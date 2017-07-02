#include "../inc/DomagRunner.h"

#include "TLogger.h"

#include "Article.h"
#include "DepotSerializer.h"
#include "HomeContainerCatalog.h"

namespace depot
{
namespace main
{

DomagRunner::DomagRunner()
{
  LOG << "Initializing domag";
  depot::serialize::DepotSerializer serializer;
  std::ifstream database{databaseFileName};
  try
  {
    serializer.deserialize(database);
  }
  catch (const depot::serialize::DepotSerializer::InvalidVersion &ex)
  {
    LOG << "Database version is invalid so there will be no data";
  }
  LOG << "Domag initalized";
}

DomagRunner::~DomagRunner()
{
  LOG << "Serialize db";
  depot::serialize::DepotSerializer serializer;
  std::ofstream database{databaseFileName};
  serializer.serialize(database);
  LOG << "Database serialized";
  ::depot::HomeContainerCatalog catalog;
  catalog.clearAllContainers();
  depot::Article::clearTopLevelArticles();
  LOG << "Database cleared";
}
}
}
