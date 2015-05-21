#pragma once

#include <string>

namespace depot
{

class IThing
{
public:
  class NameEmptyException
  {
  };
  virtual std::string getName() const noexcept = 0;
  virtual void setName(std::string n) = 0;

  virtual ~IThing() {};
};

class Thing : public IThing
{
public:
  Thing();

  std::string getName() const noexcept override;
  void setName(std::string n) override;
private:
    std::string name;
};

}
