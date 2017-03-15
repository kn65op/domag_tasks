#include <TLogger.h>
#include <gtest/gtest.h>

using namespace testing;

class ConfigurableEventListener : public TestEventListener
{

protected:
  TestEventListener *eventListener;

public:
  /**
   * Show the names of each test case.
   */
  bool showTestCases = false;

  /**
   * Show the names of each test.
   */
  bool showTestNames = false;

  /**
   * Show each success.
   */
  bool showSuccesses = false;

  /**
   * Show each failure as it occurs. You will also see it at the bottom after the full suite is run.
   */
  bool showInlineFailures = false;

  /**
   * Show the setup of the global environment.
   */
  bool showEnvironment = false;
  bool showAllTestsStarts = false;

  explicit ConfigurableEventListener(TestEventListener *theEventListener) : eventListener(theEventListener)
  {
  }

  virtual ~ConfigurableEventListener()
  {
    delete eventListener;
  }

  virtual void OnTestProgramStart(const UnitTest &unit_test)
  {
    eventListener->OnTestProgramStart(unit_test);
  }

  virtual void OnTestIterationStart(const UnitTest &unit_test, int iteration)
  {
    if (showAllTestsStarts)
    {
      eventListener->OnTestIterationStart(unit_test, iteration);
    }
  }

  virtual void OnEnvironmentsSetUpStart(const UnitTest &unit_test)
  {
    if (showEnvironment)
    {
      eventListener->OnEnvironmentsSetUpStart(unit_test);
    }
  }

  virtual void OnEnvironmentsSetUpEnd(const UnitTest &unit_test)
  {
    if (showEnvironment)
    {
      eventListener->OnEnvironmentsSetUpEnd(unit_test);
    }
  }

  virtual void OnTestCaseStart(const TestCase &test_case)
  {
    if (showTestCases)
    {
      eventListener->OnTestCaseStart(test_case);
    }
  }

  virtual void OnTestStart(const TestInfo &test_info)
  {
    if (showTestNames)
    {
      eventListener->OnTestStart(test_info);
    }
  }

  virtual void OnTestPartResult(const TestPartResult &result)
  {
    eventListener->OnTestPartResult(result);
  }

  virtual void OnTestEnd(const TestInfo &test_info)
  {
    if ((showInlineFailures && test_info.result()->Failed()) || (showSuccesses && !test_info.result()->Failed()))
    {
      eventListener->OnTestEnd(test_info);
    }
  }

  virtual void OnTestCaseEnd(const TestCase &test_case)
  {
    if (showTestCases)
    {
      eventListener->OnTestCaseEnd(test_case);
    }
  }

  virtual void OnEnvironmentsTearDownStart(const UnitTest &unit_test)
  {
    if (showEnvironment)
    {
      eventListener->OnEnvironmentsTearDownStart(unit_test);
    }
  }

  virtual void OnEnvironmentsTearDownEnd(const UnitTest &unit_test)
  {
    if (showEnvironment)
    {
      eventListener->OnEnvironmentsTearDownEnd(unit_test);
    }
  }

  virtual void OnTestIterationEnd(const UnitTest &unit_test, int iteration)
  {
    eventListener->OnTestIterationEnd(unit_test, iteration);
  }

  virtual void OnTestProgramEnd(const UnitTest &unit_test)
  {
    eventListener->OnTestProgramEnd(unit_test);
  }
};

using namespace ::testing;

int main(int argc, char **argv)
{
  TLogger::LoggerFacade logger(TLogger::LoggerType::FILE, TLogger::LogFileOnEntry::OVERRIDE,
                               TLogger::LogFileOnExit::DO_NOTHING, "tests");
  ::testing::InitGoogleTest(&argc, argv);

  testing::TestEventListeners &listeners = testing::UnitTest::GetInstance()->listeners();
  auto default_printer = listeners.Release(listeners.default_result_printer());

  ConfigurableEventListener *listener = new ConfigurableEventListener(default_printer);
  listeners.Append(listener);

  return RUN_ALL_TESTS();
}
