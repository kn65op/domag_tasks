#include "Item.hpp"
#include "Article.hpp"

#include <TLogger.h>

using depot::Item;
using depot::ConsumeHistory;
using depot::AbstractContainer;

Item::Item(std::weak_ptr<IArticle> thing_of, const PurcaseDetails & details) :
  Item{thing_of, details, std::nullopt}
{
}

Item::Item(std::weak_ptr<IArticle> thing_of, const PurcaseDetails & details, const Date& best_before) :
    Item{thing_of, details, OptionalDate{best_before}}
{
}

Item::Item(std::weak_ptr<IArticle> thing_of, const PurcaseDetails & details, const OptionalDate& best_before) :
  thing{thing_of},
  bestBefore{best_before}
{
  if (thing.expired())
  {
    throw ArticleCannotBeEmpty();
  }
  savePurcaseDetails(details);
}

void Item::savePurcaseDetails(const PurcaseDetails &details)
{
  if (details.amount == 0.0)
  {
    throw AmountCannotBeZero{};
  }
  LOG << "Create: " << thing.lock()->getName() << " in amount " << details.amount;
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
    LOG << "Trying to consume " << amount << " when there is only " << quantity << " available";
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

std::optional<std::shared_ptr<AbstractContainer>> Item::getStorehauseImpl() const
{
  if (storehause.expired())
  {
    LOG << "Item has no storehause";
    throw NoStorehause();
  }
  return storehause.lock();
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

Item::OptionalDate Item::getBestBefore() const
{
  return bestBefore;
}

void Item::setBestBefore(const OptionalDate& best_before)
{
  bestBefore = best_before;
}
