#include "../inc/Article.h"

#include <TLogger.h>

using depot::Article;

std::string Article::getName()const noexcept
{
  return name;
}

Article::Article()
{
  LOG << "Unnamed article created";
  name = "Unnamed article";
}

void Article::setName(std::string n)
{
  if (n.empty())
  {
    throw NameEmptyException();
  }
  name = n;
}