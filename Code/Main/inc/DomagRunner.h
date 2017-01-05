#pragma once

#include <memory>
#include <string>

namespace ui
{
class ContainersPresenter;
}

namespace depot
{
namespace main
{

class DomagRunner
{
public:
  DomagRunner(std::unique_ptr<ui::ContainersPresenter>);
  ~DomagRunner();

  void run();

private:
  void printMenu();
  int getOption();

  const std::string databaseFileName = "database.domag";
  std::unique_ptr<ui::ContainersPresenter> containersPresenter;
};
}
}