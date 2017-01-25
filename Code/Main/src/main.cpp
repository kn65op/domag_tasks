#include <string>

#include "../inc/DomagRunner.h"

#include "libdomagcli/CliCallback.hpp"
#include "libdomagcli/CliStarter.hpp"

#include "TLogger.h"

std::string getLogFileName()
{
  return "domag";
}

int main()
{
  TLogger::LoggerFacade logger(TLogger::LoggerType::FILE, TLogger::LogFileOnEntry::OVERRIDE,
                               TLogger::LogFileOnExit::DO_NOTHING, getLogFileName());

  LOG << "Start runner";
  depot::main::DomagRunner runner;

  domagcli::CliCallback::initializeCallback();

  LOG << "Starting CLI";
  startCli();
}
