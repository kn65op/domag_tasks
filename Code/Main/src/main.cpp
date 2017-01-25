#include <memory>
#include <string>

#include "../inc/DomagRunner.h"

#include "libdomagcli/CliCallback.hpp"
#include "libdomagcli/CliStarter.hpp"

#include "presenters/ConsoleContainersPresenter.hpp"
#include "presenters/Presenters.hpp"

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

  auto presenters = std::make_unique<ui::Presenters>(std::make_unique<cui::ConsoleContainersPresenter>());

  domagcli::CliCallback::initializeCallback(std::move(presenters));

  LOG << "Starting CLI";
  startCli();
}
