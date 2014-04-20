#ifndef CONSUMEHISTORY_H
#define CONSUMEHISTORY_H

#include <vector>
#include <utility>
#include <boost/date_time/gregorian/gregorian.hpp>

namespace depot
{

class ConsumeHistory
{
public:
  using Date = boost::gregorian::date;
  using ConsumeHistoryEntry = std::pair<double, Date>;
  using ConsumeHistoryList = std::vector<ConsumeHistoryEntry>;
  
  void push_back(int amount, Date date);
  void push_back(const ConsumeHistoryEntry &entry);
  
  ConsumeHistoryList getList() const;
private:
  ConsumeHistoryList entries;
};

}

#endif // CONSUMEHISTORY_H
