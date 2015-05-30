#include "depot/inc/ConsumeHistory.h"
#include <TLogger.h>

using namespace depot;

void ConsumeHistory::push_back(const Entry &entry)
{
  LOG << "Add new consume";
  entries.push_back(entry);
}

void ConsumeHistory::push_back(double amount, Date date)
{
  LOG << "Add new consume";
  push_back(Entry{amount, date});
}

ConsumeHistory::List ConsumeHistory::getAllConsumes() const
{
  return entries;
}