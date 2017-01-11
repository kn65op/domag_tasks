#include "cli/common.h"
#include "cli/console.h"
#include "domagCli.h"

void startCli()
{
  domag cli_EmptyCli;
  cli::Shell cli_Shell(cli_EmptyCli);
  cli::ResourceString welcome_message;
  welcome_message.SetString(cli::ResourceString::LANG_EN, "Eloszka\n");
  cli_Shell.SetWelcomeMessage(welcome_message);
  cli::Console cli_Console(false);
  cli_Shell.Run(cli_Console);
}