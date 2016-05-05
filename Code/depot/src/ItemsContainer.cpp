#include "ItemsContainer.h"

#include "TLogger.h"

using depot::ItemsContainer;

void ItemsContainer::addItem(std::unique_ptr<IItem> item)
{
  LOG << "Add item";
  item->setStorehause(getSharedPointer());
  items.push_back(std::move(item));
}

const ItemsContainer::SelectedItems ItemsContainer::getItems() const
{
  LOG << "selecting all items";
  SelectedItems allItems;
  for (auto & item : items)
  {
    allItems.push_back(item.get());
  }
  return allItems;
}

const ItemsContainer::SelectedItems ItemsContainer::getNonConsumedItems() const
{
  LOG << "selecting non consumed items";
  SelectedItems non_consumed;
  for (auto & item : items)
  {
    if (item->getQuantity() > 0)
    {
      non_consumed.push_back(item.get());
    }
  }
  return non_consumed;
}

depot::IItem::Ptr ItemsContainer::removeItem(const IItem* to_remove)
{
  LOG << "removing item";
  auto it = items.end();
  if ((it = std::find(items.begin(), items.end(), to_remove)) != items.end())
  {
    auto ret = std::move(*it);
    items.erase(it);
    ret->setStorehause({});
    return ret;
  }
  LOG << "no item to remove";
  throw NoSuchElement();
}
