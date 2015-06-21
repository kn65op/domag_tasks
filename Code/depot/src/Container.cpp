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

void Container::addContainer(ContainerInside container)
{
  LOG << "add container";
  containers.push_back(container);
  container->storehause = shared_from_this();
}

Container::ContainerInside Container::removeContainer(ContainerInside container)
{
  LOG << "Remove container";
  auto container_position = containers.end();
  if ((container_position = std::find(containers.begin(), containers.end(), container)) != containers.end())
  {
    auto ret = *container_position;
    containers.erase(container_position);
    container->storehause = nullptr;
    return ret;
  }
  LOG << "not found container";
  throw NoSuchElement();
}

const Container::Containers& Container::getContainers() const
{
  return containers;
}

void Container::removeFromContainer()
{
  storehause->removeContainer(shared_from_this());
}

void Container::addToContainer(std::shared_ptr<AbstractContainer> container)
{
}

std::shared_ptr<AbstractContainer> Container::getStorehauseImpl() const
{
  if (storehause)
  {
    return storehause;
  }
  throw LiesNowhere();
}