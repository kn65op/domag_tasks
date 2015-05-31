#pragma once

#include <string>

namespace depot
{

class IArticle
{
public:
  class NameEmptyException
  {
  };
  virtual std::string getName() const noexcept = 0;
  virtual void setName(const std::string& n) = 0;
  virtual std::string getUnit() const = 0;
  virtual void setUnit(const std::string& u) = 0;

  virtual ~IArticle() {};
};

class Article : public IArticle
{
public:
  Article();

  std::string getName() const noexcept override;
  void setName(const std::string& n) override;
  std::string getUnit() const override;
  void setUnit(const std::string& u) override;
private:
  std::string name;
  std::string unit;
};

}
