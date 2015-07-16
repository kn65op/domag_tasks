#include "../inc/DepotSerializer.h"
#include "../inc/Article.h"

using namespace depot::serialize;

void DepotSerializer::serialize(std::ostream& out)
{
  out << "Version: 1\n";
  for (auto & article : depot::TopLevelArticles::getTopLevelArticles())
  {
    out << "Article\n";
  }
}
