#pragma once

#include <memory>

#include "AbstractContainer.h"

namespace depot
{

class Storable
{
public:
  std::weak_ptr<AbstractContainer> getStorehause() const
  {
    return getStorehauseImpl();
  }

  virtual ~Storable() = default;

private:
  virtual std::weak_ptr<AbstractContainer> getStorehauseImpl() const = 0;
};

}