#include "../inc/Thing.h"

using depot::Thing;

std::string Thing::getName()const noexcept
{
  return name;
}

Thing::Thing()
{
  name = "Unnamed thing";
}

void Thing::setName(std::string n)
{
  if (n.empty())
  {
    throw NameEmptyException();
  }
  name = n;
}