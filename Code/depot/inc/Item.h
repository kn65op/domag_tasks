#ifndef ITEM_H
#define ITEM_H
#include "Container.h"

#include <memory>
#include <boost/date_time/gregorian/gregorian.hpp>

namespace depot
{

class IThing;

class IItem
{
public:
  using Date = boost::gregorian::date;

  virtual void buy(double amount, double price, Date bdate) = 0;
  virtual double getQuantity() const = 0;
  virtual double getPricePerUnit() const = 0;
  virtual void consume(double amount) = 0;
  virtual boost::gregorian::date getBuyDate() const = 0;

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

  void buy(double amount, double price = 0, Date bdate = boost::gregorian::day_clock::local_day()) override;
  double getQuantity() const override;
  double getPricePerUnit() const override;
  void consume(double amount) override;
  boost::gregorian::date getBuyDate() const override;

private:
  bool buyed = false;
  double quantity = 0;
  double price_per_unit = 0;
  Date buy_date;
};

}

#endif // ITEM_H

