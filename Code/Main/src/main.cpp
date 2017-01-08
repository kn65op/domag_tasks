/*#include <string>

#include "../inc/DomagRunner.h"
#include "TLogger.h"

std::string getLogFileName()
{
  return "domag";
}

int main()
{
  TLogger::LoggerFacade logger(TLogger::LoggerType::FILE, TLogger::LogFileOnEntry::OVERRIDE,
TLogger::LogFileOnExit::DO_NOTHING, getLogFileName());

  LOG << "main";

  depot::main::DomagRunner runner;
}
*/

#include "cli/common.h"
#include "cli/console.h"
#include "domagCli.h"

int main(void)
{
  domag cli_EmptyCli;
  cli::Shell cli_Shell(cli_EmptyCli);
  cli::ResourceString welcome_message;
  welcome_message.SetString(cli::ResourceString::LANG_EN, "Eloszka\n");
  cli_Shell.SetWelcomeMessage(welcome_message);
  cli::Console cli_Console(false);
  cli_Shell.Run(cli_Console);
}