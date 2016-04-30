#include "gtest/gtest.h"
#include "depot/inc/ConsumeHistory.h"

using namespace ::testing;
using depot::ConsumeHistory;

struct ConsumeHistoryTest : public Test
{
  using date = boost::gregorian::date;
  ConsumeHistory ch;
  date now = boost::gregorian::day_clock::local_day();
};

TEST_F(ConsumeHistoryTest, pushBackByDoubleAndDateShouldAddConsume)
{
  ch.push_back(1.1, now);
  
  ConsumeHistory::List list;
  ConsumeHistory::Entry entry{1.1, now};
  list.push_back(entry);
  
  EXPECT_EQ(list, ch.getAllConsumes());
}

TEST_F(ConsumeHistoryTest, pushBackByEntryShouldAddConsume)
{
  ConsumeHistory::Entry entry{1.1, now};
  ConsumeHistory::List list;
  list.push_back(entry);
  ch.push_back(entry);
  
  EXPECT_EQ(list, ch.getAllConsumes());
}