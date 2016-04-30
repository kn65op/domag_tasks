#include "Container.h"
#include <algorithm>
#include <TLogger.h>

using namespace depot;

template<typename Entity> typename depot::HierarchicalClass<Entity>::EntitiesContainer depot::HierarchicalClass<Entity>::top_level_entities;

void Container::addItem(Item item)
{
  LOG << "Add item";
  item->setStorehause(shared_from_this());
  items.push_back(std::move(item));
}

const Container::SelectedItems Container::getItems() const
{
  LOG << "selecting all items";
  Container::SelectedItems allItems;
  for (auto & item : items)
  {
    allItems.push_back(item.get());
  }
  return allItems;
}

const Container::SelectedItems Container::getNonConsumedItems() const
{
  LOG << "selecting non consumed items";
  Container::SelectedItems non_consumed;
  for (auto & item : items)
  {
    if (item->getQuantity() > 0)
    {
      non_consumed.push_back(item.get());
    }
  }
  return non_consumed;
}

Container::Item Container::removeItem(const ItemPtr to_remove)
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

Container::Item Container::removeItem(const ItemReference to_remove)
{
  LOG << "removing item by reference";
  return removeItem(to_remove.get());
}

std::weak_ptr<AbstractContainer> Container::getStorehauseImpl() const
{
  return getPrecedentEntity();
}

std::string Container::getName() const
{
  return name;
}
