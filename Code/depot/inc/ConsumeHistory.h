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
  using Entry = std::pair<double, Date>;
  using List = std::vector<Entry>;
  
  void push_back(int amount, Date date);
  void push_back(const Entry &entry);
  
  List getList() const;
private:
  List entries;
};

}

#endif // CONSUMEHISTORY_H
