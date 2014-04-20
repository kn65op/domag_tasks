#include "depot/inc/ConsumeHistory.h"

using namespace depot;

void ConsumeHistory::push_back(const ConsumeHistoryEntry &entry)
{
  entries.push_back(entry);
}

void ConsumeHistory::push_back(int amount, Date date)
{
  push_back(ConsumeHistoryEntry{amount, date});
}

ConsumeHistory::ConsumeHistoryList ConsumeHistory::getList() const
{
  return entries;
}