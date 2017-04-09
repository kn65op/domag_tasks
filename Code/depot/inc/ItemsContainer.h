#pragma once

#include "AbstractContainer.h"

namespace depot
{

class ItemsContainer : public AbstractContainer
{
public:
  void addItem(std::unique_ptr<IItem> item) override;
  IItem::Ptr removeItem(const IItem* to_remove) override;
  const SelectedItems getItems() const override;
  const SelectedItems getNonConsumedItems() const override;

private:
  using Items = std::vector<IItem::Ptr>;

  virtual std::shared_ptr<AbstractContainer> getSharedPointer() = 0;

  Items items;
};

}
