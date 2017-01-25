#pragma once

#include <iostream>

#include "Container.h"
#include "ContainerCatalog.h"
#include "presenters/ContainersPresenter.hpp"

namespace cui
{
class ConsoleContainersPresenter : public ui::ContainersPresenter
{
public:
  void present(const depot::ContainerCatalog &catalog)
  {
    printTableLine("Container", "Inside");
    printSeparator();
    presentContainers(catalog.getTopLevelContainers(), {});
  }

private:
  void printTableLine(const std::string &firstCol, const std::string &secondCol)
  {
    std::cout << std::setw(40) << std::left << firstCol << "|" << secondCol << "\n";
  }

  void printSeparator()
  {
    std::cout << std::setfill('-') << std::setw(80) << "-"
              << "\n";
    std::cout << std::setfill(' ');
  }

  void presentContainers(const depot::ContainerCatalog::Containers &containers, const std::string &inName)
  {
    for (const auto &container : containers)
    {
      printTableLine(container->getName(), inName);
      presentContainers(container->getContainers(), inName + ":" + container->getName());
    }
  }
};
}