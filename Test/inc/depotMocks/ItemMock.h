#pragma once

#include "depot/inc/Item.h"
#include <gmock/gmock.h>

namespace depot
{
namespace ut
{

class ItemMock : public IItem
{
 public:
  MOCK_METHOD1(buy, void(const PurcaseDetails &));
  MOCK_CONST_METHOD0(getQuantity, double());
  MOCK_CONST_METHOD0(getBoughtAmount, double());
  MOCK_CONST_METHOD0(getPricePerUnit, double());
  MOCK_METHOD2(consume, void(double amount, Date date));
  MOCK_CONST_METHOD0(getBuyDate, boost::gregorian::date());
  MOCK_CONST_METHOD0(getConsumeHistory, const ConsumeHistory::List&());
  MOCK_CONST_METHOD0(getThing, std::weak_ptr<IArticle>());
  MOCK_CONST_METHOD0(getStorehauseImpl, std::weak_ptr<AbstractContainer>());
  MOCK_METHOD1(setStorehause, void(std::weak_ptr<AbstractContainer>));
};

}}  // namespace depot::ut
