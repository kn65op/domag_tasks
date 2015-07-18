#pragma once

#include "ConsumeHistory.h"
#include <memory>
#include <boost/date_time/gregorian/gregorian.hpp>
#include "Article.h"
#include "Storable.h"
#include "AbstractContainer.h"

namespace depot
{

class IItem
{
public:
  using Ptr = std::unique_ptr<IItem>;
  using Reference = std::reference_wrapper<Ptr>;

  virtual ~IItem() {}
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
  virtual std::shared_ptr<IArticle> getThing() const = 0;

  struct NoQuantityToConsume
  {
  };

  struct ItemAlreadyBuyed
  {
  };

  struct NoStorehause
  {
  };

  struct ArticleCannotBeEmpty
  {
  };
};

class Item : public IItem, public Storable
{
public:
  using Storehause = std::shared_ptr<AbstractContainer>;
  using Article = std::shared_ptr<IArticle>;

  Item(std::shared_ptr<IArticle> thing_of);

  void buy(double amount, double price = 0, Date bdate = boost::gregorian::day_clock::local_day()) override;
  double getQuantity() const override;
  double getPricePerUnit() const override;
  void consume(double amount, Date date = boost::gregorian::day_clock::local_day()) override;
  boost::gregorian::date getBuyDate() const override;
  ConsumeHistory::List getConsumeHistory() const override;
  std::shared_ptr<IArticle> getThing() const override;
  void setStorehause(Storehause store);
  void changeArticle(Article art);

private:
  std::shared_ptr<IArticle> thing;
  bool buyed = false;
  double quantity = 0;
  double price_per_unit = 0;
  Date buy_date;
  ConsumeHistory history;
  Storehause storehause;

  std::shared_ptr<AbstractContainer> getStorehauseImpl() const override;
};

}
