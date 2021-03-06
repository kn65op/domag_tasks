#pragma once

#include <vector>
#include <utility>
#include <boost/date_time/gregorian/gregorian.hpp>

namespace depot
{

class ConsumeHistory
{
public:
  using Date = boost::gregorian::date;
  using Entry = std::pair<double, Date>;
  using List = std::vector<Entry>;

  void push_back(double amount, Date date);
  void push_back(const Entry &entry);

  const List& getAllConsumes() const;

private:
  List entries;
};

}
