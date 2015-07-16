#include "../inc/DepotSerializer.h"
#include "../inc/Article.h"

using namespace depot::serialize;

void DepotSerializer::serialize(std::ostream& out)
{
  out << "Version: 1\n";
  auto top_level_articles = depot::TopLevelArticles::getTopLevelArticles();
  if (!top_level_articles.empty())
  {
    out << "Articles:\n";
    for (auto & article : top_level_articles)
    {
      out << "  - name: " + article->getName() + "\n";
    }
  }
}
