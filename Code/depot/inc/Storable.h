#pragma once

#include <memory>

#include "AbstractContainer.h"

namespace depot
{

class Storable
{
public:
  void changeContainer(std::shared_ptr<AbstractContainer> new_container)
  {
    try
    {
    }
    catch (AbstractContainer::NoSuchElement &)
    {

    }
  }

  std::shared_ptr<AbstractContainer> getStorehause() const
  {
    return getStorehauseImpl();
  }

private:
  virtual void removeFromContainer() = 0;
  virtual void addToContainer(std::shared_ptr<AbstractContainer> new_container) = 0;
  virtual std::shared_ptr<AbstractContainer> getStorehauseImpl() const = 0;
};

}