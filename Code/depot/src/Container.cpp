#include "../inc/Container.h"

using namespace depot;


void Container::addItem(std::unique_ptr<IItem> item)
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