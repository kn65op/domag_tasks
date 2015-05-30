#pragma once

#include "depot/inc/Item.h"
#include <gmock/gmock.h>

namespace depot {
namespace ut {

class ItemMock : public IItem {
 public:
  MOCK_METHOD3(buy,
      void(double amount, double price, Date bdate));
  MOCK_CONST_METHOD0(getQuantity,
      double());
  MOCK_CONST_METHOD0(getPricePerUnit,
      double());
  MOCK_METHOD2(consume,
      void(double amount, Date date));
  MOCK_CONST_METHOD0(getBuyDate,
      boost::gregorian::date());
  MOCK_CONST_METHOD0(getConsumeHistory,
      ConsumeHistory::List());
  MOCK_CONST_METHOD0(getThing,
      std::shared_ptr<IThing>());
};

}}  // namespace depot::ut
