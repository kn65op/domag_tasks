#pragma once

#include <memory>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/optional.hpp>

#include "ConsumeHistory.hpp"
#include "Article.hpp"
#include "Storable.hpp"

namespace depot
{

class AbstractContainer;

struct PurcaseDetails
{
  using Date = boost::gregorian::date;

  PurcaseDetails(double amount) : PurcaseDetails(amount, 0.0)
  { }

  PurcaseDetails(double amount, double price) : PurcaseDetails(amount, price, boost::gregorian::day_clock::local_day())
  {}

  PurcaseDetails(double amount, double price, Date date) :
    amount(amount),
    price(price),
    date(date)
  {
  }

  double amount;
  double price;
  Date date;
};

class IItem : public Storable
{
public:
  using Ptr = std::unique_ptr<IItem>;

  IItem() = default;
  IItem(const IItem &) = delete;
  IItem(const IItem &&) = delete;

  IItem & operator=(const IItem &) = delete;
  IItem & operator=(const IItem &&) = delete;

  using Date = PurcaseDetails::Date;
  using OptionalDate = std::optional<Date>;

  virtual double getQuantity() const = 0;
  virtual double getPricePerUnit() const = 0;
  virtual void consume(double amount, Date date) = 0;
  virtual Date getBuyDate() const = 0;
  virtual OptionalDate getBestBefore() const = 0;
  virtual void setBestBefore(const OptionalDate&) = 0;
  virtual const ConsumeHistory::List& getConsumeHistory() const = 0;
  virtual std::weak_ptr<IArticle> getThing() const = 0;
  virtual void setStorehause(std::weak_ptr<AbstractContainer>) = 0;
  virtual double getBoughtAmount() const = 0;

  struct NoQuantityToConsume
  {
  };

  struct NoStorehause
  {
  };

  struct ArticleCannotBeEmpty
  {
  };

  struct AmountCannotBeZero
  {
  };
};

class Item : public IItem
{
public:
  using Storehause = std::weak_ptr<AbstractContainer>;
  using Article = std::weak_ptr<IArticle>;

  Item(std::weak_ptr<IArticle> thing_of, const PurcaseDetails &);
  Item(std::weak_ptr<IArticle> thing_of, const PurcaseDetails &, const Date &);

  double getQuantity() const override;
  double getPricePerUnit() const override;
  void consume(double amount, Date date = boost::gregorian::day_clock::local_day()) override;
  boost::gregorian::date getBuyDate() const override;
  double getBoughtAmount() const override;
  const ConsumeHistory::List& getConsumeHistory() const override;
  std::weak_ptr<IArticle> getThing() const override;
  void setStorehause(Storehause store) override;
  void changeArticle(Article art);
  OptionalDate getBestBefore() const override;
  void setBestBefore(const OptionalDate &) override;

private:
  Item(std::weak_ptr<IArticle> thing_of, const PurcaseDetails &, const OptionalDate &);

  std::weak_ptr<IArticle> thing;
  double quantity = 0;
  double initialQuantity = 0;
  double price_per_unit = 0;
  Date buy_date;
  OptionalDate bestBefore;
  ConsumeHistory history;
  Storehause storehause;

  std::optional<std::shared_ptr<AbstractContainer>> getStorehauseImpl() const override;
  void savePurcaseDetails(const PurcaseDetails &);
};

inline bool operator==(IItem::Ptr &lhs, const IItem * rhs)
{
  return lhs.get() == rhs;
}

}
