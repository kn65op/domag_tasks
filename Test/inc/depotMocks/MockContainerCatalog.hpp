#pragma once

#include "gmock/gmock.h"

#include "depot/inc/ContainerCatalog.hpp"
#include "depot/inc/ItemsContainer.hpp"

struct MockContainerCatalog : public depot::ContainerCatalog
{
    MOCK_CONST_METHOD0(getTopLevelContainers, const Containers&());
    MOCK_METHOD0(clearAllContainers, void());
    MOCK_METHOD1(createTopLevelContainer, ContainerInside(const std::string&));
    MOCK_METHOD1(removeTopLevelContainer, void(ContainerInside));
    MOCK_CONST_METHOD0(getContainerForConsumedItems, depot::ItemsContainer&());
};
