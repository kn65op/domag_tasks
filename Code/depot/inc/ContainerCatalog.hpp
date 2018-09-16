#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace depot
{
class HierarchicalItemsContainer;
class ItemsContainer;

class ContainerCatalog
{
public:
    struct ContainerNotFound : public std::logic_error
    {
        using std::logic_error::logic_error;
    };

    using ContainerInside = std::shared_ptr<HierarchicalItemsContainer>;
    using Containers = std::vector<ContainerInside>;

    virtual ~ContainerCatalog() = default;

    virtual const Containers& getTopLevelContainers() const = 0;
    virtual void clearAllContainers() = 0;
    virtual ContainerInside createTopLevelContainer(const std::string&) = 0;
    virtual void removeTopLevelContainer(ContainerInside) = 0;
    virtual ItemsContainer& getContainerForConsumedItems() const = 0;
};
}
