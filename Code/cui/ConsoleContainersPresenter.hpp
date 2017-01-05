#pragma once

#include <iostream>

#include "Container.h"
#include "ContainerCatalog.h"
#include "ui/ContainersPresenter.hpp"

namespace cui
{
class ConsoleContainersPresenter : public ui::ContainersPresenter
{
public:
  void present(const depot::ContainerCatalog &catalog)
  {
    for (const auto &container : catalog.getTopLevelContainers())
    {
      std::cout << "Container: " << container->getName() << "\n";
    }
  }
};
}