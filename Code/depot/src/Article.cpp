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
  LOG << "Add dependent article" << article->name;
  articles.push_back(article);
}

Article::Articles& Article::getArticles()
{
  return articles;
}

Article::DependentArticle Article::removeDependentArticle(DependentArticle article)
{
  LOG << "Remove dependent article" << article->name;
  auto article_position = std::find(articles.begin(), articles.end(), article);
  if (article_position == articles.end())
  {
    LOG << "Not found article to remove: " << article->name;
    throw NoExistDependentArticle();
  }
  auto removed_article = articles.erase(article_position);
  return *removed_article;
}

Article::ArticlePtr Article::createArticle()
{
  return ArticlePtr(new Article());
}