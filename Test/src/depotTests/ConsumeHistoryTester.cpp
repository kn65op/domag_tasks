#include "gtest/gtest.h"
#include "depot/inc/ConsumeHistory.h"

using namespace ::testing;
using depot::ConsumeHistory;

struct ConsumeHistoryTest : public Test
{
  using date = boost::gregorian::date;
  ConsumeHistory ch;
  date now = boost::gregorian::day_clock::local_day();
  date weekBefore = boost::gregorian::day_clock::local_day() - boost::gregorian::date_duration(7);
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

TEST_F(ConsumeHistoryTest, ConsumeShouldBeOrderedByTime)
{
  ConsumeHistory::Entry laterEntry{1.1, now};
  ConsumeHistory::Entry earlierEntry{1.1, weekBefore};
  ConsumeHistory::List list{earlierEntry, laterEntry};

  ch.push_back(laterEntry);
  ch.push_back(earlierEntry);
  
  EXPECT_EQ(list, ch.getAllConsumes());
}