#pragma once

#include "ContainerCatalog.h"

namespace depot
{

class HomeContainerCatalog : public ContainerCatalog
{
public:
  const Containers& getTopLevelContainers() const override;
  void clearTopLevelContainers() override;
  ContainerInside createTopLevelContainer(const std::string& name = "Unnamed container") override;
  void removeTopLevelContainer(ContainerInside) override;
};

}
