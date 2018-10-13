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

TEST_F(RemoveAnyContainerProcedureTest, shouldThrowWhenContainerCannotBeFound)
{
    EXPECT_CALL(*catalog, removeTopLevelContainer(_)).WillOnce(Throw(MockContainerCatalog::ContainerNotFound{"Not"}));

    ASSERT_THROW(procedure.removeContainer(nullptr), depot::RemoveContainerProcedure::ContainerNotFound);
}

TEST_F(RemoveAnyContainerProcedureTest, shouldRemoveContainerWhenOnTopLevel)
{
    std::shared_ptr<depot::HierarchicalItemsContainer> continerToRemove;

    EXPECT_CALL(*catalog, removeTopLevelContainer(continerToRemove));

    procedure.removeContainer(continerToRemove);
}

TEST_F(RemoveAnyContainerProcedureTest, shuoldRemoveContainerFr)
{

}