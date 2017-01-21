#pragma once

#include <string>

namespace domagcli
{

class CliCallback
{
public:
  template <typename OutputStream, typename EndlFunction, typename OutputCommand>
  static void noArgCommand(OutputStream &out, const EndlFunction &endl, OutputCommand command)
  {
    out << command() << endl;
  }

  static std::string showAllContainers()
  {
    return getAllContainers();
  }

private:
  static std::string getAllContainers();
};
}
