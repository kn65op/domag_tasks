#ifndef ITEM_H
#define ITEM_H
#include "Container.h"

#include <memory>

namespace depot
{

class IThing;
  
class Item
{
public: 
  Item(std::shared_ptr<IThing> thing);
};

}

#endif // ITEM_H
