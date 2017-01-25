#include "CliCallback.hpp"

namespace domagcli
{

std::unique_ptr<CliCallback> CliCallback::callback = nullptr;

std::string CliCallback::getAllContainers()
{
  return "All containers";
}

void CliCallback::initializeCallback()
{
  callback = std::make_unique<CliCallback>();
}

std::string CliCallback::showAllContainers()
{
  return callback->getAllContainers();
}
}
