#include "../inc/Article.h"

#include <TLogger.h>
#include <algorithm>

using depot::Article;
using depot::TopLevelArticles;

TopLevelArticles::Container TopLevelArticles::top_level_articles;

Article::~Article()
{
}

Article::Article()
{
  LOG << "Unnamed article created";
}

Article::Article(const std::string& n) :
    name{n}
{
  LOG << "Article " << name.getContent() << "created";
}

Article::Article(const std::string& n, const std::string &u) :
    name{n},
    unit{u}
{
  LOG << "Article " << name.getContent() << "created";
}

std::string Article::getName()const noexcept
{
  return name;
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
  TopLevelArticles::removeTopLevelArticle(article);
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
  TopLevelArticles::addArticleToTopLevelArticles(article);
  return *removed_article;
}

Article::ArticlePtr Article::getPrecedentArticle()
{
  if (precedent)
  {
    return precedent;
  }
  throw NoPrecedentArticle();
}

Article::ArticlePtr Article::createDependentArticle(ArticlePtr precedent)
{
  ArticlePtr new_article{new Article()};
  precedent->addDependentArticle(new_article);
  return new_article;
}

Article::ArticlePtr Article::createDependentArticle(ArticlePtr precedent, const std::string &n)
{
  ArticlePtr new_article{new Article(n)};
  precedent->addDependentArticle(new_article);
  return new_article;
}

Article::ArticlePtr Article::createDependentArticle(ArticlePtr precedent, const std::string &n, const std::string &u)
{
  ArticlePtr new_article{new Article(n, u)};
  precedent->addDependentArticle(new_article);
  return new_article;
}

Article::ArticlePtr TopLevelArticles::createTopLevelArticle()
{
  ArticlePtr new_article{new Article()};
  addArticleToTopLevelArticles(new_article);
  return new_article;
}

Article::ArticlePtr TopLevelArticles::createTopLevelArticle(const std::string &n)
{
  ArticlePtr new_article{new Article(n)};
  addArticleToTopLevelArticles(new_article);
  return new_article;
}

Article::ArticlePtr TopLevelArticles::createTopLevelArticle(const std::string &n, const std::string &u)
{
  ArticlePtr new_article{new Article(n, u)};
  addArticleToTopLevelArticles(new_article);
  return new_article;
}

const TopLevelArticles::Container& TopLevelArticles::getTopLevelArticles()
{
  return top_level_articles;
}

void TopLevelArticles::addArticleToTopLevelArticles(ArticlePtr article)
{
  LOG << "Adding top level article";
  top_level_articles.push_back(article);
}

void TopLevelArticles::removeArticleFromTopLevelArticles(ArticlePtr article)
{
  auto article_position = std::find(top_level_articles.begin(), top_level_articles.end(), article);
  LOG << "Searching if top level article exists";
  if (article_position != top_level_articles.end())
  {
    LOG << "Removing top level article";
    top_level_articles.erase(article_position);
  }
}

void TopLevelArticles::removeTopLevelArticle(ArticlePtr article)
{
  removeArticleFromTopLevelArticles(article);
}

void TopLevelArticles::clearTopLevelArticles()
{
  top_level_articles.clear();
}