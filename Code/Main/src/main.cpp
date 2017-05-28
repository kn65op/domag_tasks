#include <string>

#include "../inc/DomagRunner.h"
#include "TLogger.h"
#include "gui/Application.hpp"

std::string getLogFileName()
{
  return "domag";
}

int main()
{
  TLogger::LoggerFacade logger(TLogger::LoggerType::FILE, TLogger::LogFileOnEntry::OVERRIDE,
                               TLogger::LogFileOnExit::DO_NOTHING, getLogFileName());

  LOG << "Start application";

  depot::main::DomagRunner runner;

  gui::Application domagApp;
}
