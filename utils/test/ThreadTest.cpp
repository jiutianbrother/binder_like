#include <iostream>

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <Logger.h>
#include <Thread.h>

using namespace std;

Logger log("/tmp/test.log");

static void * func1(void *para)
{
    int *ret = new int;
    *ret = 1;
    return ret;
}

static void * func2(void *para)
{
    LOG("para = %d", *(int*)para);
    return NULL;
}

class LoggerTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(LoggerTest);
    
    CPPUNIT_TEST(testThreadName);
    CPPUNIT_TEST(testRun);

    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

    void testThreadName();
    void testRun();
};


CPPUNIT_TEST_SUITE_REGISTRATION(LoggerTest);

void LoggerTest::setUp()
{
}

void LoggerTest::tearDown()
{
}

void LoggerTest::testThreadName()
{
    Thread t1;
    Thread t2;
    Thread t3("t3");

    CPPUNIT_ASSERT(t1.getThreadName() == "Thread-1");
    CPPUNIT_ASSERT(t2.getThreadName() == "Thread-2");
    CPPUNIT_ASSERT(t3.getThreadName() == "t3");
}

void LoggerTest::testRun()
{
    Thread t1, t2;
    void *ret;
    int para = 2;

    CPPUNIT_ASSERT(true == t1.setThreadFunc(func1, NULL));
    CPPUNIT_ASSERT(true == t1.start());

    CPPUNIT_ASSERT(true == t2.setThreadFunc(func2, &para));
    CPPUNIT_ASSERT(true == t2.start());
    
    CPPUNIT_ASSERT(true == t1.join(&ret));
    CPPUNIT_ASSERT(1 == *(int *)ret);
    delete (int *)ret;

    CPPUNIT_ASSERT(true == t2.join(NULL));
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
