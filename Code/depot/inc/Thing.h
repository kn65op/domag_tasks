#ifndef THING_H
#define THING_H

#include <string>

class Thing
{
public:
  class NameEmptyException
  {
  };
  
  Thing();
  
  std::string getName() const noexcept;
  void setName(std::string n);
private:
    std::string name;
};

#endif // THING_H
