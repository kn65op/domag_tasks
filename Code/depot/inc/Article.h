#pragma once

#include <string>
#include <memory>
#include <map>
#include <vector>

#include "String/UniqueString.hpp"

namespace depot
{

class IArticle
{
public:
  class NameEmptyException
  {
  };
  class NoExistDependentArticle
  {
  };
  class NoPrecedentArticle
  {
  };
  virtual std::string getName() const noexcept = 0;
  virtual void setName(const std::string& n) = 0;
  virtual std::string getUnit() const = 0;
  virtual void setUnit(const std::string& u) = 0;

  IArticle() = default;
  IArticle(const IArticle&) = delete;
  IArticle& operator=(const IArticle&) = delete;

  virtual ~IArticle() {};
};

class Article : public IArticle, public std::enable_shared_from_this<Article>
{
public:
  using DependentArticle = std::shared_ptr<Article>;
  using Articles = std::vector<DependentArticle>;
  using ArticlePtr = std::shared_ptr<Article>;
  using ArticleWeakPtr = std::weak_ptr<Article>;
  using AllArticles = std::map<std::string, ArticleWeakPtr>;

  ~Article();
  static ArticlePtr createDependentArticle(ArticlePtr precedent);

  std::string getName() const noexcept override;
  void setName(const std::string& n) override;
  std::string getUnit() const override;
  void setUnit(const std::string& u) override;
  void addDependentArticle(DependentArticle article);
  Articles& getArticles();
  DependentArticle removeDependentArticle(DependentArticle article);
  ArticlePtr getPrecedentArticle();

private:
  friend class TopLevelArticles;
  static const int UniqueStringCategory = 1;

  THelper::String::UniqueStdCategorizedString<UniqueStringCategory> name;
  std::string unit;
  Articles articles;
  ArticlePtr precedent;

  Article();
  Article(const std::string &name);
};

class TopLevelArticles
{
public:
  using ArticlePtr = Article::ArticlePtr;
  using Container = std::vector<ArticlePtr>;

  static Article::ArticlePtr createTopLevelArticle();
  static Article::ArticlePtr createTopLevelArticle(const std::string& name);
  static void removeTopLevelArticle(ArticlePtr article);
  static const Container& getTopLevelArticles();
  static void clearTopLevelArticles();

private:
  friend class Article;

  static Container top_level_articles;

  static void addArticleToTopLevelArticles(ArticlePtr article);
  static void removeArticleFromTopLevelArticles(ArticlePtr article);
};

}
