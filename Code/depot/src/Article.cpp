#include "../inc/Article.h"

#include <TLogger.h>
#include <algorithm>

using depot::Article;
using depot::TopLevelArticles;

TopLevelArticles::Container TopLevelArticles::top_level_articles;

Article::~Article()
{
  LOG << "Removing: " << name.getContent() << "\n";
}

Article::Article()
{
  LOG << "Unnamed article created";
}

Article::Article(const std::string& n, const std::string &u) :
    name{n},
    unit{u}
{
  LOG << "Article " << name.getContent() << "with unit: " << unit << "created";
}

void Article::checkPassedName(const std::string& n)
{
  if (n.empty())
  {
    LOG << "Set default category name";
    THelper::String::UniqueStdCategorizedString<Article::UniqueStringCategory>::setDefaultName("Unnamed Category");
  }
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
  checkIfArticleCanBeAdded(article);
  dependent_articles.push_back(article);
  article->precedent = shared_from_this();
  TopLevelArticles::removeTopLevelArticle(article);
}

void Article::checkIfArticleCanBeAdded(const DependentArticle article) const
{
  LOG << "Check if make self dependency";
  if (article.get() == this)
  {
    LOG << "Making self dependency";
    throw CannotMakeDependent("Trying to make yourself dependent");
  }
  try
  {
    LOG << "Check adding as dependent one of precedents";
    if(auto precedent_real = precedent.lock())
    {
      precedent_real->checkIfArticleCanBeAdded(article);
    }
  }
  catch(const CannotMakeDependent&)
  {
    LOG << "Making circular dependency";
    throw CannotMakeDependent("Trying to add as dependent article one of precedents");
  }
}

Article::Articles& Article::getArticles()
{
  return dependent_articles;
}

Article::DependentArticle Article::removeDependentArticle(DependentArticle article)
{
  LOG << "Remove dependent article" << article->name.getContent();
  const auto article_position = std::find(dependent_articles.begin(), dependent_articles.end(), article);
  if (article_position == dependent_articles.end())
  {
    LOG << "Not found article to remove: " << article->name.getContent();
    throw NoExistDependentArticle();
  }
  auto removed_article = dependent_articles.erase(article_position);
  (*removed_article)->precedent.reset();
  TopLevelArticles::addArticleToTopLevelArticles(article);
  return *removed_article;
}

Article::ArticlePtr Article::getPrecedentArticle()
{
  if (auto precedent_real = precedent.lock())
  {
    return precedent_real;
  }
  throw NoPrecedentArticle();
}

Article::ArticlePtr Article::createDependentArticle(ArticlePtr precedent, const std::string &n, const std::string &u)
{
  ArticlePtr new_article{new Article(n, u)};
  precedent->addDependentArticle(new_article);
  return new_article;
}

Article::ArticlePtr TopLevelArticles::createTopLevelArticle(const std::string &n, const std::string &u)
{
  Article::checkPassedName(n);
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
  LOG << "Clearing top level articles";
  top_level_articles.clear();
}