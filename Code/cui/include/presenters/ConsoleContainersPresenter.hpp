#pragma once

#include <iostream>
#include <sstream>
#include <string>

#include "Container.h"
#include "ContainerCatalog.h"
#include "presenters/ContainersPresenter.hpp"

namespace cui
{
class ConsoleContainersPresenter : public ui::ContainersPresenter
{
public:
  std::string present(const depot::ContainerCatalog &catalog)
  {
    return printTableLine("Container", "Inside") + printSeparator() +
           presentContainers(catalog.getTopLevelContainers(), {});
  }

private:
  std::string printTableLine(const std::string &firstCol, const std::string &secondCol)
  {
    std::stringstream out;
    out << std::setw(40) << std::left << firstCol << "|" << secondCol << "\n";
    return out.str();
  }

  std::string printSeparator()
  {
    std::stringstream out;
    out << std::setfill('-') << std::setw(80) << "-"
        << "\n";
    out << std::setfill(' ');
    return out.str();
  }

  std::string presentContainers(const depot::ContainerCatalog::Containers &containers, const std::string &inName)
  {
    std::string ret;
    for (const auto &container : containers)
    {
      ret += printTableLine(container->getName(), inName) +
             presentContainers(container->getContainers(), inName + ":" + container->getName());
    }
    return ret;
  }
};
}