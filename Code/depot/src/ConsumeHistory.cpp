#include "depot/inc/ConsumeHistory.h"

using namespace depot;

void ConsumeHistory::push_back(const Entry &entry)
{
  entries.push_back(entry);
}

void ConsumeHistory::push_back(double amount, Date date)
{
  push_back(Entry{amount, date});
}

ConsumeHistory::List ConsumeHistory::getAllConsumes() const
{
  return entries;
}