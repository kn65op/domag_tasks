#pragma once

#include "gmock/gmock.h"

#include "MockContainer.hpp"
#include "depot/inc/ItemsContainer.h"

class MockHierarchicalItemsContainer : public MockContainer, public virtual depot::HierarchicalContainer
{
  public:
    MOCK_METHOD0(createDependentContainer, std::shared_ptr<depot::HierarchicalContainer>());
    MOCK_METHOD1(createDependentContainer, std::shared_ptr<depot::HierarchicalContainer>(const std::string&));
    MOCK_METHOD1(addContainer, void(std::shared_ptr<depot::HierarchicalContainer>));
    MOCK_METHOD1(removeContainer, std::shared_ptr<depot::HierarchicalContainer>(
                                      std::shared_ptr<depot::HierarchicalContainer>));
};
