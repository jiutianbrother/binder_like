#include <iostream>

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <Logger.h>

using namespace std;

class LoggerTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(LoggerTest);
    
    CPPUNIT_TEST(testFilePath);
    CPPUNIT_TEST(testLog);

    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

    void testFilePath();
    void testLog();
};


CPPUNIT_TEST_SUITE_REGISTRATION(LoggerTest);

void LoggerTest::setUp()
{
}

void LoggerTest::tearDown()
{
}

void LoggerTest::testFilePath()
{
    Logger logger;
    
    CPPUNIT_ASSERT(logger.getLogFilePath() == logger.getDefaultLogFile());

    Logger logger1("test.log");
    CPPUNIT_ASSERT(logger1.getLogFilePath() == "/tmp/binder_like/log/test.log");

    Logger logger2("/tmp/test.log");
    CPPUNIT_ASSERT(logger2.getLogFilePath() == "/tmp/test.log");
}

Logger log("/tmp/test.log");

void LoggerTest::testLog()
{
    LOG("This is a test log");
    LOG("This is a test log:%s", "Hello, test");
}

int main(int argc, char **argv)
{
    CppUnit::TextUi::TestRunner runner;
    CppUnit::TestFactoryRegistry &registry = \
	CppUnit::TestFactoryRegistry::getRegistry();
    
    runner.addTest(registry.makeTest());
    runner.run();
    
    return 0;
}
