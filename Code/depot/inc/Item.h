#pragma once

#include "ConsumeHistory.h"
#include <memory>
#include <boost/date_time/gregorian/gregorian.hpp>
#include "Article.h"
#include "Storable.h"
#include "AbstractContainer.h"

namespace depot
{

class IItem : public Storable
{
public:
  using Ptr = std::unique_ptr<IItem>;
  using Reference = std::reference_wrapper<Ptr>;

  IItem() = default;
  IItem(const IItem &) = delete;
  IItem(const IItem &&) = delete;

  IItem & operator=(const IItem &) = delete;
  IItem & operator=(const IItem &&) = delete;

  using Date = boost::gregorian::date;

  virtual void buy(double amount, double price, Date bdate) = 0;
  virtual double getQuantity() const = 0;
  virtual double getPricePerUnit() const = 0;
  virtual void consume(double amount, Date date) = 0;
  virtual boost::gregorian::date getBuyDate() const = 0;
  virtual ConsumeHistory::List getConsumeHistory() const = 0;
  virtual std::weak_ptr<IArticle> getThing() const = 0;
  virtual void setStorehause(std::weak_ptr<AbstractContainer>) = 0;
  virtual double getBoughtAmmount() const = 0;

  struct NoQuantityToConsume
  {
  };

  struct ItemAlreadyBought
  {
  };

  struct NoStorehause
  {
  };

  struct ArticleCannotBeEmpty
  {
  };
};

class Item : public IItem
{
public:
  using Storehause = std::weak_ptr<AbstractContainer>;
  using Article = std::weak_ptr<IArticle>;

  explicit Item(std::weak_ptr<IArticle> thing_of);

  void buy(double amount, double price = 0, Date bdate = boost::gregorian::day_clock::local_day()) override;
  double getQuantity() const override;
  double getPricePerUnit() const override;
  void consume(double amount, Date date = boost::gregorian::day_clock::local_day()) override;
  boost::gregorian::date getBuyDate() const override;
  double getBoughtAmmount() const override;
  ConsumeHistory::List getConsumeHistory() const override;
  std::weak_ptr<IArticle> getThing() const override;
  void setStorehause(Storehause store) override;
  void changeArticle(Article art);

private:
  std::weak_ptr<IArticle> thing;
  bool bought = false;
  double quantity = 0;
  double initialQuantity = 0;
  double price_per_unit = 0;
  Date buy_date;
  ConsumeHistory history;
  Storehause storehause;

  std::weak_ptr<AbstractContainer> getStorehauseImpl() const override;
};

inline bool operator==(IItem::Ptr &lhs, const IItem * rhs)
{
  return lhs.get() == rhs;
}

}
