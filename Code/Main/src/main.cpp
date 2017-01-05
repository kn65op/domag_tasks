#include <string>

#include "DomagRunner.h"
#include "TLogger.h"
#include "cui/ConsoleContainersPresenter.hpp"

std::string getLogFileName()
{
  return "domag";
}

int main()
{
  TLogger::LoggerFacade logger(TLogger::LoggerType::FILE, TLogger::LogFileOnEntry::OVERRIDE,
                               TLogger::LogFileOnExit::DO_NOTHING, getLogFileName());

  LOG << "main";

  depot::main::DomagRunner runner{std::make_unique<cui::ConsoleContainersPresenter>()};
  runner.run();
  return 0;
}
