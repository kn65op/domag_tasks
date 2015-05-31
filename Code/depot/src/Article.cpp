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

void Article::setName(const std::string& n)
{
  if (n.empty())
  {
    throw NameEmptyException();
  }
  name = n;
}

void Article::setUnit(const std::string& u)
{
  unit = u;
}

std::string Article::getUnit() const
{
  return unit;
}