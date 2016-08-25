#pragma once

#include <string>
#include <memory>
#include <map>
#include <vector>
#include "HierarchicalClass.h"
#include "THelper/String/UniqueString.hpp"

namespace depot
{

class IArticle
{
public:
  class NameEmptyException : public std::logic_error
  {
  public:
    NameEmptyException(const std::string& msg = "Cannot set empty name") :
        std::logic_error(msg)
    {
    }
  };
  class NoExistDependentArticle : public std::logic_error
  {
  public:
    NoExistDependentArticle(const std::string& msg = "There is no such dependent article") :
        std::logic_error(msg)
    {
    }
  };
  class NoPrecedentArticle : public std::logic_error
  {
  public:
    NoPrecedentArticle(const std::string& msg = "There is no precedent article") :
        std::logic_error(msg)
    {
    }
  };
  class CannotMakeDependent : public std::logic_error
  {
  public:
    CannotMakeDependent(const std::string& msg) :
        std::logic_error(msg)
    {
    }
  };
  class InvalidArticle : public std::logic_error
  {
  public:
    InvalidArticle(const std::string& msg = "Invalid pointer (not Article) was passed to make shared") :
        std::logic_error(msg)
    {
    }
  };

  virtual std::string getName() const noexcept = 0;
  virtual void setName(const std::string& n) = 0;
  virtual std::string getUnit() const = 0;
  virtual void setUnit(const std::string& u) = 0;

  IArticle() = default;
  IArticle(const IArticle&) = delete;
  IArticle& operator=(const IArticle&) = delete;

  virtual ~IArticle()
  {
  }
  ;
};

class Article : public IArticle, public std::enable_shared_from_this<Article>, HierarchicalClass< Article>
{
public:
  using DependentArticle = std::shared_ptr<Article>;
  using Articles = std::vector<DependentArticle>;
  using ArticlePtr = std::shared_ptr<Article>;
  using PrecedentArticle = std::weak_ptr<Article>;
  using ArticleWeakPtr = std::weak_ptr<Article>;
  using AllArticles = std::map<std::string, ArticleWeakPtr>;

  using NoPrecedentException = IArticle::NoPrecedentArticle;
  using CircularDependencyException = IArticle::CannotMakeDependent;
  using NoInferiorException = IArticle::NoExistDependentArticle;

  ~Article();
  ArticlePtr createDependentArticle(const std::string& name = "", const std::string& unit = "");

  std::string getName() const noexcept override;
  void setName(const std::string& n) override;
  std::string getUnit() const override;
  void setUnit(const std::string& u) override;
  void addDependentArticle(DependentArticle article);
  const Articles& getArticles() const;
  DependentArticle removeDependentArticle(DependentArticle article);
  ArticlePtr getPrecedentArticle() const;
  static void doCreationChecks(const std::string& name, const std::string &)
  {
    checkPassedName(name);
  }

  static HierarchicalClass::EntitySharedPtr createTopLevelArticle(const std::string& name = "", const std::string& unit = "")
  {
    return createTopLevelEntity(name, unit);
  }

  static void removeTopLevelArticle(HierarchicalClass::EntitySharedPtr article)
  {
    removeTopLevelEntity(article);
  }

  static const HierarchicalClass::EntitiesContainer& getTopLevelArticles()
  {
    return getTopLevelEntities();
  }

  static void clearTopLevelArticles()
  {
    clearTopLevelEntites();
  }

  std::shared_ptr<Article> makeSharedPointer() override
  {
    return shared_from_this();
  }

private:
  friend class TopLevelArticles;
  friend class HierarchicalClass<Article> ;
  static const int UniqueStringCategory = 1;

  THelper::String::UniqueStdCategorizedString<UniqueStringCategory> name;
  std::string unit;

  Article(const std::string &name, const std::string &unit);

  static void checkPassedName(const std::string& name);
};

}
