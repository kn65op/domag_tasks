#pragma once

#include <memory>
#include "ItemsContainer.h"

namespace depot
{

class ConsumedItems : public ItemsContainer, public std::enable_shared_from_this<ConsumedItems>
{
public:
  std::string getName() const override;
private:
  std::shared_ptr<AbstractContainer> getSharedPointer() override;
};
}
