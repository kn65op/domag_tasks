#pragma once

#include "gmock/gmock.h"

#include "MockContainer.hpp"
#include "depot/inc/ItemsContainer.h"

class MockHierarchicalItemsContainer : public MockContainer, public virtual depot::HierarchicalItemsContainer
{
  public:
    MOCK_METHOD0(createDependentContainer, std::shared_ptr<depot::HierarchicalItemsContainer>());
    MOCK_METHOD1(createDependentContainer, std::shared_ptr<depot::HierarchicalItemsContainer>(const std::string&));
    MOCK_METHOD1(addContainer, void(std::shared_ptr<depot::HierarchicalItemsContainer>));
    MOCK_METHOD1(removeContainer, std::shared_ptr<depot::HierarchicalItemsContainer>(
                                      std::shared_ptr<depot::HierarchicalItemsContainer>));
};
