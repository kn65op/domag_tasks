#pragma once

#include <memory>
#include <string>

namespace ui
{
class Presenters;
}

namespace domagcli
{

class CliCallback
{
public:
  template <typename OutputStream, typename Endl, typename OutputCommand>
  static void noArgCommand(OutputStream &out, const Endl &endl, OutputCommand command)
  {
    if (callback)
    {
      out << command() << endl;
    }
    else
    {
      out << "Error: not initialized, please contact author" << endl;
    }
  }

  static std::string showAllContainers();

  static void initializeCallback(std::unique_ptr<ui::Presenters>);

private:
  static std::unique_ptr<CliCallback> callback;

  std::string getAllContainers();

  CliCallback(std::unique_ptr<ui::Presenters>);
  std::unique_ptr<ui::Presenters> presenters;
};
}
