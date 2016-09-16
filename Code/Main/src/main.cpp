#include <string>

#include "../inc/DomagRunner.h"
#include "TLogger.h"

std::string getLogFileName()
{
  return "domag";
}

int main()
{
  TLogger::LoggerFacade logger(TLogger::LoggerType::FILE, TLogger::LogFileOnEntry::OVERRIDE, TLogger::LogFileOnExit::DO_NOTHING, getLogFileName());

  LOG << "main";

  depot::main::DomagRunner runner;
}
