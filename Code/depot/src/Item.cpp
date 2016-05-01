#include "Item.h"
#include "Article.h"
#include <TLogger.h>

using depot::Item;
using depot::ConsumeHistory;
using depot::AbstractContainer;

Item::Item(std::weak_ptr<IArticle> thing_of) :
    thing{thing_of}
{
  if (thing.expired())
  {
    throw ArticleCannotBeEmpty();
  }
}

void Item::buy(const PurcaseDetails &details)
{
  if (bought)
  {
    LOG << "Item already buyed";
    throw ItemAlreadyBought();
  }
  bought = true;
  buy_date = details.date;
  initialQuantity = quantity = details.amount;
  price_per_unit = details.price / details.amount;
}

double Item::getBoughtAmount() const
{
  return initialQuantity;
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

const ConsumeHistory::List& Item::getConsumeHistory() const
{
  return history.getAllConsumes();
}

Item::Article Item::getThing() const
{
  return thing;
}

std::weak_ptr<AbstractContainer> Item::getStorehauseImpl() const
{
  if (storehause.expired())
  {
    LOG << "Item has no storehause";
    throw NoStorehause();
  }
  return storehause;
}

void Item::setStorehause(Storehause store)
{
  storehause = store;
}

void Item::changeArticle(Article art)
{
  if (art.expired())
  {
    throw ArticleCannotBeEmpty{};
  }
  thing = art;
}