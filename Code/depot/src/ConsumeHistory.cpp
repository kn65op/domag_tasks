#include <algorithm>
#include "depot/inc/ConsumeHistory.hpp"
#include <TLogger.h>

using namespace depot;

void ConsumeHistory::push_back(const Entry &entry)
{
  LOG << "Add new consume";
  entries.push_back(entry);
  static const auto comparator = [](const auto& lhs, const auto& rhs)
  {
    return lhs.second < rhs.second;
  };

  if (!std::is_sorted(entries.begin(), entries.end(), comparator))
  {
    std::sort(entries.begin(), entries.end(), comparator);
  }
}

void ConsumeHistory::push_back(double amount, Date date)
{
  push_back(Entry{amount, date});
}

const ConsumeHistory::List &ConsumeHistory::getAllConsumes() const
{
  return entries;
}
