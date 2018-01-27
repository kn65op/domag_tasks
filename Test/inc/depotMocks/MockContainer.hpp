#pragma once

#include "gmock/gmock.h"

#include "depot/inc/AbstractContainer.h"

class MockContainer : public virtual depot::AbstractContainer
{
  public:
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
};
