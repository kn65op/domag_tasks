#ifndef CONTAINER_H
#define CONTAINER_H

#include "Item.h"
#include <memory>
#include <functional>

namespace depot
{
  
class Container
{
public:
  struct NoSuchElement
  {
  };
  typedef std::unique_ptr<IItem> Item;
  typedef std::vector<Item> Items;
  typedef std::vector<std::reference_wrapper<IItem>> SelectedItems;
  void addItem(std::unique_ptr<IItem> item);
  Item removeItem(const Item & to_remove);
  
  const Items & getItems() const;
  const SelectedItems getNonConsumedItems() const;
private:
  Items items;
};

}

#endif // CONTAINER_H
