#include "gtest/gtest.h"

#include "MockContainerCatalog.hpp"
#include "MockHierarchicalItemsContainer.hpp"

#include "depot/inc/RemoveContainerProcedure.hpp"

using namespace testing;

struct RemoveAnyContainerProcedureTest : public Test
{
    std::shared_ptr<MockContainerCatalog> catalog = std::make_shared<NiceMock<MockContainerCatalog>>();
    depot::RemoveAnyContainerProcedure procedure{catalog};
};

TEST_F(RemoveAnyContainerProcedureTest, shouldThrowWhenContainerIsNullptr)
{
    ASSERT_THROW(procedure.removeContainer(nullptr), depot::RemoveContainerProcedure::ContainerNotFound);
}

TEST_F(RemoveAnyContainerProcedureTest, shouldThrowWhenContainerCannotBeFound)
{
    std::shared_ptr<MockHierarchicalItemsContainer> containerToRemove = std::make_shared<NiceMock<MockHierarchicalItemsContainer>>();
    EXPECT_CALL(*catalog, removeTopLevelContainer(_)).WillOnce(Throw(MockContainerCatalog::ContainerNotFound{"Not"}));

    ASSERT_THROW(procedure.removeContainer(containerToRemove), depot::RemoveContainerProcedure::ContainerNotFound);

}

TEST_F(RemoveAnyContainerProcedureTest, shouldRemoveContainerWhenOnTopLevel)
{
    std::shared_ptr<MockHierarchicalItemsContainer> containerToRemove = std::make_shared<NiceMock<MockHierarchicalItemsContainer>>();
    ON_CALL(*containerToRemove, getPrecedent()).WillByDefault(Return(std::nullopt));

    EXPECT_CALL(*catalog, removeTopLevelContainer(Eq(containerToRemove)));

    procedure.removeContainer(containerToRemove);
}

TEST_F(RemoveAnyContainerProcedureTest, shouldRemoveContainerWhenNotOnTopLevel)
{
    auto parentContainer = std::make_shared<StrictMock<MockHierarchicalItemsContainer>>();
    std::shared_ptr<MockHierarchicalItemsContainer> containerToRemove = std::make_shared<StrictMock<MockHierarchicalItemsContainer>>();
    EXPECT_CALL(*containerToRemove, getPrecedent()).WillOnce(Return(parentContainer));
    EXPECT_CALL(*parentContainer, removeContainer(Eq(containerToRemove))).WillOnce(Return(containerToRemove));

    procedure.removeContainer(containerToRemove);

    Mock::VerifyAndClearExpectations(containerToRemove.get());
    Mock::VerifyAndClearExpectations(parentContainer.get());
}