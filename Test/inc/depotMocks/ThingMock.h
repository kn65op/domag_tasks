#pragma once

#include "depot/inc/Thing.h"
#include "gmock/gmock.h"

namespace depot
{
namespace ut
{

struct ThingMock : public IThing
{
  MOCK_CONST_METHOD0(getNameMock, std::string());
  std::string getName() const noexcept
  {
    return getNameMock();
  }
  MOCK_METHOD1(setName, void(std::string));
};

}}              