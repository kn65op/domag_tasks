#include "depot/inc/Item.h"
#include <TLogger.h>
#include "../inc/Article.h"

using namespace depot;

Item::Item(std::shared_ptr<IArticle> thing_of) :
    thing{thing_of}
{
}

void Item::buy(double amount, double price, Date bdate)
{
  if (buyed)
  {
    LOG << "Item already buyed";
    throw ItemAlreadyBuyed();
  }
  buyed = true;
  buy_date = bdate;
  quantity = amount;
  price_per_unit = price / quantity;
}

double Item::getQuantity() const
{
  return quantity;
}

double Item::getPricePerUnit() const
{
  return price_per_unit;
}

void Item::consume(double amount, Date date)
{
  if (amount > quantity)
  {
    LOG << "There is no amount available to consume";
    throw NoQuantityToConsume();
  }
  quantity -= amount;
  history.push_back(amount, date);
}

boost::gregorian::date Item::getBuyDate() const
{
  return buy_date;
}

ConsumeHistory::List Item::getConsumeHistory() const
{
  return history.getAllConsumes();
}

std::shared_ptr<IArticle> Item::getThing() const
{
  return thing;
}