#include <gtest/gtest.h>
#include <TLogger.h>

using namespace ::testing;

int main(int argc, char **argv) {
  TLogger::LoggerFacade logger(TLogger::LoggerType::FILE, TLogger::LogFileOnEntry::OVERRIDE, TLogger::LogFileOnExit::DO_NOTHING, "tests");
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
