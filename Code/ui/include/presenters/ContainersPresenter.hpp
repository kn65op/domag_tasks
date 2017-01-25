#pragma once

namespace depot
{
class ContainerCatalog;
}

namespace ui
{

class ContainersPresenter
{
public:
  virtual ~ContainersPresenter() = default;

  virtual std::string present(const depot::ContainerCatalog &) = 0;
};
}