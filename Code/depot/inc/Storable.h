#pragma once

#include <memory>

#include "AbstractContainer.h"

namespace depot
{

class Storable
{
public:
  std::shared_ptr<AbstractContainer> getStorehause() const
  {
    return getStorehauseImpl();
  }

private:
  virtual std::shared_ptr<AbstractContainer> getStorehauseImpl() const = 0;
};

}