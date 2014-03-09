#include "../inc/Thing.h"

std::string Thing::getName() const
{
  return name;
}

Thing::Thing()
{
  name = "Unnamed thing";
}