#include "../inc/Container.h"
#include <algorithm>
#include <TLogger.h>

using namespace depot;


void Container::addItem(Item item)
{
  LOG << "Add item";
  items.push_back(std::move(item));
}

const Container::Items& Container::getItems() const
{
  return items;
}

const Container::SelectedItems Container::getNonConsumedItems() const
{
  LOG << "selecting non consumed items";
  Container::SelectedItems selected;
  for (auto & item : items)
  {
    if (item->getQuantity() > 0)
    {
      selected.push_back(*item);
    }
  }
  return selected;
}

Container::Item Container::removeItem(const Item & to_remove)
{
  LOG << "removing item";
  auto it = items.end();
  if ((it = std::find(items.begin(), items.end(), to_remove)) != items.end())
  {
    auto ret = std::move(*it);
    items.erase(it);
    return ret;
  }
  LOG << "no item to remove";
  throw NoSuchElement();
}
