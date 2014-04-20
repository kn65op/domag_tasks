#include "gtest/gtest.h"
#include "depot/inc/ConsumeHistory.h"

using namespace ::testing;
using depot::ConsumeHistory;

//TODO: needs    

struct ConsumeHistoryTest : public Test
{
  using date = boost::gregorian::date;
  ConsumeHistory ch;
  date now = boost::gregorian::day_clock::local_day();
};

TEST_F(ConsumeHistoryTest, pushBackByDoubleAndDateShouldAddConsume)
{
  ch.push_back(1, now);
  
  ConsumeHistory::ConsumeHistoryList list;
  ConsumeHistory::ConsumeHistoryEntry entry{1, now};
  list.push_back(entry);
  
  EXPECT_EQ(list, ch.getList());
}

TEST_F(ConsumeHistoryTest, pushBackByEntryShouldAddConsume)
{
  ConsumeHistory::ConsumeHistoryEntry entry{1, now};
  ConsumeHistory::ConsumeHistoryList list;
  list.push_back(entry);
  ch.push_back(entry);
  
  EXPECT_EQ(list, ch.getList());
}