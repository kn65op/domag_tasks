#pragma once

#include "gmock/gmock.h"
#include "../../../Code/depot/inc/Article.h"

namespace depot
{
namespace ut
{

struct ArticleMock : public IArticle
{
  MOCK_CONST_METHOD0(getNameMock, std::string());
  std::string getName() const noexcept
  {
    return getNameMock();
  }
  MOCK_METHOD1(setName, void(const std::string& ));
  MOCK_CONST_METHOD0(getUnit, std::string());
  MOCK_METHOD1(setUnit, void(const std::string& ));
};

}}