#include "../inc/Article.h"

#include <TLogger.h>
#include <algorithm>

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


void Article::addDependentArticle(DependentArticle article)
{
  articles.push_back(article);
}

Article::Articles& Article::getArticles()
{
  return articles;
}

Article::DependentArticle Article::removeDependentArticle(DependentArticle article)
{
  auto article_position = std::find(articles.begin(), articles.end(), article);
  if (article_position == articles.end())
  {
    throw NoExistDependentArticle();
  }
  auto removed_article = articles.erase(article_position);
  return *removed_article;
}