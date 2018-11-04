#pragma once

#include "gmock/gmock.h"

#include "MockContainer.hpp"
#include "depot/inc/ItemsContainer.hpp"

class MockHierarchicalItemsContainer : public virtual depot::HierarchicalItemsContainer
{
public:
    MOCK_CONST_METHOD0(getContainers, const Containers&());
    MOCK_METHOD0(createDependentContainer, std::shared_ptr<depot::HierarchicalItemsContainer>());
    MOCK_METHOD1(createDependentContainer, std::shared_ptr<depot::HierarchicalItemsContainer>(const std::string&));
    MOCK_METHOD1(addContainer, void(std::shared_ptr<depot::HierarchicalItemsContainer>));
    MOCK_METHOD1(removeContainer,
                 std::shared_ptr<depot::HierarchicalItemsContainer>(std::shared_ptr<depot::HierarchicalItemsContainer>));
    MOCK_METHOD1(addItem, void(depot::IItem*));
    void addItem(std::unique_ptr<depot::IItem> item) override
    {
        addItem(item.get());
    }

    MOCK_METHOD1(removeItem, depot::IItem::Ptr(const depot::IItem*));
    depot::IItem::Ptr removeItem_(const depot::IItem* item)
    {
        return removeItem(item);
    }

    MOCK_CONST_METHOD0(getItems, const SelectedItems());
    MOCK_CONST_METHOD0(getNonConsumedItems, const SelectedItems());
    MOCK_CONST_METHOD0(getName, std::string());
    MOCK_CONST_METHOD0(createDependentContainer, std::shared_ptr<AbstractContainer>());
    MOCK_METHOD0(getSharedPointer, std::shared_ptr<AbstractContainer>());
    MOCK_CONST_METHOD0(getPrecedent, std::optional<std::shared_ptr<HierarchicalItemsContainer>>());
};
