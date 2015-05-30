#include "../inc/Thing.h"
#include <TLogger.h>

using depot::Thing;

std::string Thing::getName()const noexcept
{
  return name;
}

Thing::Thing()
{
  LOG << "Unnamed thing created";
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