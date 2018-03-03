#include "ConsumedItems.hpp"

using depot::ConsumedItems;

std::string ConsumedItems::getName() const
{
  return "Consumed items";
}

std::shared_ptr<depot::AbstractContainer> ConsumedItems::getSharedPointer()
{
  return shared_from_this();
}
