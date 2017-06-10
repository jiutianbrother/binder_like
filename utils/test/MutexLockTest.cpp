#include <iostream>

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <Logger.h>
#include <Thread.h>
#include <MutexLock.h>

using namespace std;

Logger log("/tmp/test.log");

static int count = 0;
static MutexLock lock = MutexLock(false);

static void * func(void *para)
{
    Thread *t = (Thread *)para;

    MutexGuard guard(&lock);

    ++count;
    LOG("%s: count = %d", t->getThreadName().c_str(), count);

    return NULL;
}

class LoggerTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(LoggerTest);
    
    CPPUNIT_TEST(testLock);

    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

    void testLock();
};


CPPUNIT_TEST_SUITE_REGISTRATION(LoggerTest);

void LoggerTest::setUp()
{
}

void LoggerTest::tearDown()
{
}


void LoggerTest::testLock()
{
#define	    THREAD_NUM	    100
    int i;
    Thread *t[THREAD_NUM];

    for (i = 0; i < THREAD_NUM; ++i){
	t[i] = new Thread();
	CPPUNIT_ASSERT(true == t[i]->setThreadFunc(func, t[i]));
	CPPUNIT_ASSERT(true == t[i]->start());
    }

    for (i = 0; i < THREAD_NUM; ++i){
	LOG("waiting Thread-%d...", i+1);
	CPPUNIT_ASSERT(true == t[i]->join(NULL));
	delete t[i];
	t[i] = NULL;
    }
    
    CPPUNIT_ASSERT(THREAD_NUM == count);
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
