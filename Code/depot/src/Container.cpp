#include "Container.h"
#include <algorithm>
#include <TLogger.h>

using namespace depot;

template<typename Entity> typename depot::HierarchicalClass<Entity>::EntitiesContainer depot::HierarchicalClass<Entity>::top_level_entities;

void Container::addItem(Item item)
{
  LOG << "Add item";
  items.push_back(std::move(item));
}

const Container::Items& Container::getItems() const
{
  return items;
}

const Container::SelectedItems Container::getNonConsumedItems()
{
  LOG << "selecting non consumed items";
  Container::SelectedItems non_consumed;
  for (auto & item : items)
  {
    if (item->getQuantity() > 0)
    {
      non_consumed.push_back(std::ref(item));
    }
  }
  return non_consumed;
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

Container::Item Container::removeItem(const ItemReference to_remove)
{
  LOG << "removing item by reference";
  return removeItem(to_remove.get());
}

std::shared_ptr<AbstractContainer> Container::getStorehauseImpl() const
{
  return getPrecedentEntity();
}

std::string Container::getName() const
{
  return name;
}
