#include "../inc/Container.h"
#include <algorithm>

using namespace depot;


void Container::addItem(Item item)
{
  items.push_back(std::move(item));
}

const Container::Items& Container::getItems() const
{
  return items;
}

const Container::SelectedItems Container::getNonConsumedItems() const
{
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
  auto it = items.end();
  if ((it = std::find(items.begin(), items.end(), to_remove)) != items.end())
  {
    auto ret = std::move(*it);
    items.erase(it);
    return ret;
  }
  throw NoSuchElement();
}
