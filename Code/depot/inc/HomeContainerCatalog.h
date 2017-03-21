#pragma once

#include <memory>

#include "ContainerCatalog.h"

namespace depot
{
class ConsumedItems;

class HomeContainerCatalog : public ContainerCatalog
{
public:
  const Containers& getTopLevelContainers() const override;
  void clearAllContainers() override;
  ContainerInside createTopLevelContainer(const std::string& name = "Unnamed container") override;
  void removeTopLevelContainer(ContainerInside) override;
  ItemsContainer& getContainerForConsumedItems() const override;
private:
  static std::shared_ptr<ConsumedItems> consumedItems;
};

}
