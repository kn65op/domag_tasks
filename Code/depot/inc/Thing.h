#ifndef THING_H
#define THING_H

#include <string>

class Thing
{
public:
  Thing();
  
  std::string getName() const;
private:
    std::string name;
};

#endif // THING_H
