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
  typedef std::vector<std::unique_ptr<IItem>> Items;
  typedef std::vector<std::reference_wrapper<IItem>> SelectedItems;
  void addItem(std::unique_ptr<IItem> item);
  
  const Items & getItems() const;
  const SelectedItems getNonConsumedItems() const;
private:
  Items items;
};

}

#endif // CONTAINER_H
