#pragma once

#include <string>
#include <memory>
#include <vector>

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

  virtual ~IArticle() {};
};

class Article : public IArticle, public std::enable_shared_from_this<Article>
{
public:
  using DependentArticle = std::shared_ptr<Article>;
  using Articles = std::vector<DependentArticle>;
  using ArticlePtr = std::shared_ptr<Article>;

  static ArticlePtr createArticle();

  std::string getName() const noexcept override;
  void setName(const std::string& n) override;
  std::string getUnit() const override;
  void setUnit(const std::string& u) override;
  void addDependentArticle(DependentArticle article);
  Articles& getArticles();
  DependentArticle removeDependentArticle(DependentArticle article);
  ArticlePtr getPrecedentArticle();

private:
  std::string name;
  std::string unit;
  Articles articles;
  ArticlePtr precedent;

  Article();
};

}
