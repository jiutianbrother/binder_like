#include <iostream>

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <Logger.h>
#include <Thread.h>
#include <CondVariable.h>

using namespace std;

Logger log("/tmp/test.log");

static CondVariable cond;
static MutexLock lock = MutexLock(false);
static bool flag = false;

static void * func1(void *para)
{
    bool ret;
    MutexGuard guard(&lock);

    LOG("get lock");
    ret = cond.wait(&lock);
    
    LOG("recv signal, ret=%d, flag=%d", ret, flag);
    return NULL;
}

static void * func2(void *para)
{
    bool ret;

    LOG("start");
    MutexGuard guard(&lock);
    flag = true;
    ret = cond.signal();
    LOG("send signal, ret=%d, flag=%d", ret, flag);

    return NULL;
}

class LoggerTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(LoggerTest);
    
    CPPUNIT_TEST(testCondVariable);

    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

    void testCondVariable();
};


CPPUNIT_TEST_SUITE_REGISTRATION(LoggerTest);

void LoggerTest::setUp()
{
}

void LoggerTest::tearDown()
{
}

void LoggerTest::testCondVariable()
{
    struct timespec ts;
    void *ret;

    Thread t1("t1");
    Thread t2("t2");

    t1.setThreadFunc(func1, NULL);
    t2.setThreadFunc(func2, NULL);

    CPPUNIT_ASSERT(true == t1.start());
    ts.tv_sec = 0;
    ts.tv_nsec = 1000;
    nanosleep(&ts, NULL);
    CPPUNIT_ASSERT(true == t2.start());

    CPPUNIT_ASSERT(true == t2.join(&ret));
    CPPUNIT_ASSERT(true == t1.join(&ret));
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
