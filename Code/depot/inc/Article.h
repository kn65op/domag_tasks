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
    using DependentArticle = std::shared_ptr<IArticle>;
    using Articles = std::vector<DependentArticle>;

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
      using std::logic_error::logic_error;
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
  virtual const Articles& getArticles() const = 0;
  virtual DependentArticle createDependentArticle(const std::string& name = "", const std::string& unit = "") = 0;
  virtual void addDependentArticle(DependentArticle article) = 0;
  virtual DependentArticle getPrecedentArticle() const = 0;

  IArticle() = default;
  IArticle(const IArticle&) = delete;
  IArticle& operator=(const IArticle&) = delete;

  virtual ~IArticle() = default;
};

class Article : HierarchicalClass< Article, IArticle>, public IArticle, public std::enable_shared_from_this<Article>
{
public:
  using ArticlePtr = std::shared_ptr<Article>;
  using PrecedentArticle = std::weak_ptr<Article>;
  using ArticleWeakPtr = std::weak_ptr<Article>;
  using AllArticles = std::map<std::string, ArticleWeakPtr>;

  using NoPrecedentException = IArticle::NoPrecedentArticle;
  using CircularDependencyException = IArticle::CannotMakeDependent;
  using NoInferiorException = IArticle::NoExistDependentArticle;

  ~Article();

  DependentArticle createDependentArticle(const std::string& name = "", const std::string& unit = "") override;

  std::string getName() const noexcept override;
  void setName(const std::string& n) override;
  std::string getUnit() const override;
  void setUnit(const std::string& u) override;
  void addDependentArticle(DependentArticle article) override;
  const Articles& getArticles() const override;
  DependentArticle removeDependentArticle(DependentArticle article);
  DependentArticle getPrecedentArticle() const override;
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
  friend class HierarchicalClass<Article, IArticle>;
  static const int UniqueStringCategory = 1;

  THelper::String::UniqueStdCategorizedString<UniqueStringCategory> name;
  std::string unit;

  Article(const std::string &name, const std::string &unit);

  static void checkPassedName(const std::string& name);
};

}
