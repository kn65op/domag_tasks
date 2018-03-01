#pragma once

#include "../../../Code/depot/inc/Article.h"
#include "gmock/gmock.h"

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
    MOCK_METHOD1(setName, void(const std::string&));
    MOCK_CONST_METHOD0(getUnit, std::string());
    MOCK_METHOD1(setUnit, void(const std::string&));
    MOCK_CONST_METHOD0(getArticles, const Articles&());
    MOCK_METHOD2(createDependentArticle, DependentArticle(const std::string&, const std::string&));
    MOCK_METHOD1(addDependentArticle, void(DependentArticle));
    MOCK_CONST_METHOD0(getPrecedentArticle, DependentArticle());
};
}
}
