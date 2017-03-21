#pragma once

#include <vector>
#include <string>
#include <memory>

#include "Item.h"

namespace depot
{

class AbstractContainer
{
public:
  using SelectedItems = std::vector<depot::IItem*>;

  struct NoSuchElement
  {
  };

  virtual ~AbstractContainer() = default;

  virtual void addItem(std::unique_ptr<IItem>) = 0;
  virtual IItem::Ptr removeItem(const IItem* ) = 0;
  virtual const SelectedItems getItems() const = 0;
  virtual const SelectedItems getNonConsumedItems() const = 0;
  virtual std::string getName() const = 0;
};
}


