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
  class NameEmptyException : public std::logic_error
  {
  public:
    NameEmptyException(const std::string& msg = "Cannot set empty name") : std::logic_error(msg)
    {}
  };
  class NoExistDependentArticle : public std::logic_error
  {
  public:
    NoExistDependentArticle(const std::string& msg = "There is no such dependent article") : std::logic_error(msg)
    {}
  };
  class NoPrecedentArticle : public std::logic_error
  {
  public:
    NoPrecedentArticle(const std::string& msg = "There is no precedent article") : std::logic_error(msg)
    {}
  };
  class CannotMakeDependent : public std::logic_error
  {
  public:
    CannotMakeDependent(const std::string& msg) : std::logic_error(msg)
    {}
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
  using PrecedentArticle = std::weak_ptr<Article>;
  using ArticleWeakPtr = std::weak_ptr<Article>;
  using AllArticles = std::map<std::string, ArticleWeakPtr>;

  ~Article();
  static ArticlePtr createDependentArticle(ArticlePtr precedent);
  static ArticlePtr createDependentArticle(ArticlePtr precedent, const std::string& name, const std::string& unit = "");

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
  Articles dependent_articles;
  PrecedentArticle precedent;

  Article();
  Article(const std::string &name);
  Article(const std::string &name, const std::string &unit);

  void checkPassedName(const std::string& name) const;
  void checkIfArticleCanBeAdded(const DependentArticle) const;
};

class TopLevelArticles
{
public:
  using ArticlePtr = Article::ArticlePtr;
  using Container = std::vector<ArticlePtr>;

  static Article::ArticlePtr createTopLevelArticle();
  static Article::ArticlePtr createTopLevelArticle(const std::string& name, const std::string& unit = "");
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
