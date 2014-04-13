#ifndef ITEM_H
#define ITEM_H
#include "Container.h"

#include <memory>

namespace depot
{

class IThing;

class IItem
{
public:
  virtual void buy(double amount, double price = 0) = 0;
  virtual double getQuantity() const = 0;
  virtual double getPricePerUnit() const = 0;
  virtual void consume(double amount) = 0;
  
  struct NoQuantityToConsume
  {
  };
  
  struct ItemAlreadyBuyed
  {
  };
};
  
class Item : public IItem
{
public: 
  Item(std::shared_ptr<IThing> thing);
  
  void buy(double amount, double price = 0) override;
  double getQuantity() const override;
  double getPricePerUnit() const override;
  void consume(double amount) override;
  
private:
  bool buyed = false;
  double quantity = 0;
  double price_per_unit = 0;
};

}

#endif // ITEM_H
