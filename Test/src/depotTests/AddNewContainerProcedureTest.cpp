#include "gtest/gtest.h"

#include "MockContainerCatalog.hpp"
#include "MockHierarchicalItemsContainer.hpp"

#include "depot/inc/AddNewContainerProcedure.hpp"

using namespace testing;

struct AddTopLevelContainerProcedureTest : public Test
{
    std::shared_ptr<MockContainerCatalog> catalog = std::make_shared<MockContainerCatalog>();
    depot::AddTopLevelContainerProcedure procedure{catalog};
};

TEST_F(AddTopLevelContainerProcedureTest, shouldAddNewTopLevelContainer)
{
    const std::string name{"asd"};

    std::shared_ptr<depot::HierarchicalItemsContainer> addedContainer =
        std::make_shared<MockHierarchicalItemsContainer>();
    EXPECT_CALL(*catalog, createTopLevelContainer(name)).WillOnce(Return(addedContainer));
    EXPECT_THAT(procedure.add(name).lock(), Eq(addedContainer));
}

struct AddDependentContainerProcedureTest : public Test
{
    std::shared_ptr<MockHierarchicalItemsContainer> container = std::make_shared<MockHierarchicalItemsContainer>();
    const std::string name{"aqwe"};
};

TEST_F(AddDependentContainerProcedureTest, shouldAddNewDependendContainer)
{
    depot::AddDependentContainerProcedure procedure(container);

    std::shared_ptr<depot::HierarchicalItemsContainer> addedContainer =
        std::make_shared<MockHierarchicalItemsContainer>();
    EXPECT_CALL(*container, createDependentContainer(name)).WillOnce(Return(addedContainer));

    EXPECT_THAT(procedure.add(name).lock(), Eq(addedContainer));
}
