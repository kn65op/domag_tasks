#pragma once

#include <memory>

class ContainersPresenter;

namespace ui
{
class Presenters
{
public:
  Presenters(std::unique_ptr<ContainersPresenter> contP) : containersPresenter{std::move(contP)}
  {
  }
  std::unique_ptr<ContainersPresenter> containersPresenter;
};
}
