#pragma once

#include <iostream>

#include "ContainerCatalog.h"
#include "ui/ContainersPresenter.hpp"

namespace cui
{
class ConsoleContainersPresenter : public ui::ContainersPresenter
{
public:
  void present(const depot::ContainerCatalog &)
  {
    std::cout << "!@#!@#!@\n";
  }
};
}