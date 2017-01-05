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

  virtual void present(const depot::ContainerCatalog &) = 0;
};
}