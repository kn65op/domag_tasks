#pragma once

#include "AbstractContainer.h"

namespace depot
{

class Storable
{
public:
  void changeContainer(AbstractContainer& new_container)
  {
    try
    {
    }
    catch (AbstractContainer::NoSuchElement &)
    {

    }
  }

private:
  virtual void removeFromContainer() = 0;
  virtual void addToContainer(AbstractContainer& new_container) = 0;
};

}