#pragma once

#include <memory>
#include <string>

namespace domagcli
{

class CliCallback
{
public:
  template <typename OutputStream, typename EndlFunction, typename OutputCommand>
  static void noArgCommand(OutputStream &out, const EndlFunction &endl, OutputCommand command)
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

  static void initializeCallback();

private:
  static std::string getAllContainers();

  static std::unique_ptr<CliCallback> callback;
};
}
