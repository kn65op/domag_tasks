#ifndef THING_H
#define THING_H

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
};

class Thing : public IThing
{
public:
  Thing();
  
  std::string getName() const noexcept;
  void setName(std::string n);
private:
    std::string name;
};

}

#endif // THING_H
