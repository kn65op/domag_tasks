#include <string>

#include "../inc/DatabaseGuard.hpp"
#include "TLogger.h"
#include "gui/Application.hpp"

std::string getLogFileName()
{
  return "domag";
}

int main()
{
  TLogger::LoggerFacade logger(TLogger::LoggerType::STDERR, TLogger::LogFileOnEntry::OVERRIDE,
                               TLogger::LogFileOnExit::DO_NOTHING, getLogFileName());

  LOG << "Start application";

  depot::main::DatabaseGuard dbGuard;

  gui::Application domagApp;
}
