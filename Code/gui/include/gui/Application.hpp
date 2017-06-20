#pragma once

#include <memory>

namespace gui
{
class Application
{
  public:
    Application();

  private:
    void prepareView();
    void newContainer();
    void openNewContainerDialog();
};
}
