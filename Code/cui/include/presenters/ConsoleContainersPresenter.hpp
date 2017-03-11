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
    auto Id = 1;
    return printTableLine("Id", "Container", "Inside") + printSeparator() +
           presentContainers(Id, catalog.getTopLevelContainers(), {});
  }

private:
  std::string printTableLine(const std::string &i, const std::string &firstCol, const std::string &secondCol)
  {
    std::stringstream out;
    constexpr auto separater = '|';
    out << std::setw(4) << i << separater << std::setw(40) << std::left << firstCol << separater << secondCol << "\n";
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

  std::string
  presentContainers(int &id, const depot::ContainerCatalog::Containers &containers, const std::string &inName)
  {
    std::string ret;
    for (const auto &container : containers)
    {
      ret += printTableLine(std::to_string(id++), container->getName(), inName);
      ret += presentContainers(id, container->getContainers(), inName + ":" + container->getName());
    }
    return ret;
  }
};
}