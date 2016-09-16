#include <string>

#include "TLogger.h"

std::string getLogFileName()
{
  return "domag";
}

int main()
{
  TLogger::LoggerFacade logger(TLogger::LoggerType::FILE, TLogger::LogFileOnEntry::CREATE_WITH_DATE, TLogger::LogFileOnExit::REMOVE, getLogFileName());
}
