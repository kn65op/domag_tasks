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
  LOG << "Add dependent article" << article->name.getContent();
  articles.push_back(article);
  article->precedent = shared_from_this();
}

Article::Articles& Article::getArticles()
{
  return articles;
}

Article::DependentArticle Article::removeDependentArticle(DependentArticle article)
{
  LOG << "Remove dependent article" << article->name.getContent();
  auto article_position = std::find(articles.begin(), articles.end(), article);
  if (article_position == articles.end())
  {
    LOG << "Not found article to remove: " << article->name.getContent();
    throw NoExistDependentArticle();
  }
  auto removed_article = articles.erase(article_position);
  (*removed_article)->precedent = nullptr;
  return *removed_article;
}

Article::ArticlePtr Article::createArticle()
{
  return ArticlePtr(new Article());
}

Article::ArticlePtr Article::getPrecedentArticle()
{
  if (precedent)
  {
    return precedent;
  }
  throw NoPrecedentArticle();
}