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

    EXPECT_CALL(*catalog, createTopLevelContainer(name)).WillOnce(Return(nullptr));
    procedure.add(name);
}

struct AddDependentContainerProcedureTest : public Test
{
    std::shared_ptr<MockHierarchicalItemsContainer> container = std::make_shared<MockHierarchicalItemsContainer>();
    const std::string name{"aqwe"};
};

TEST_F(AddDependentContainerProcedureTest, shouldAddNewDependendContainer)
{
    depot::AddDependentContainerProcedure procedure(container);

    EXPECT_CALL(*container, createDependentContainer(name));

    procedure.add(name);
}
