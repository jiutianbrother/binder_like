/*
 * File: Thread.cpp
 * Description: 线程类实现
 * Author: fanxiaoyu
 *
 */

#include <string>

#include <Thread.h>
#include <Logger.h>


using namespace std;

extern Logger log;

/*****************************************************/
/*		    public			     */
/*****************************************************/
Thread::Thread(std::string name, ThreadAttr attr)
    : mThreadAttr(attr)
    , mThreadFunc(NULL)
    , mPara(NULL)
    , mThreadName(name)
    , mIsStarted(false)
{
    if (name.empty())
	setDefaultThreadName();
    LOG("create thread(%s)", mThreadName.c_str());

    setPthreadAttr();
}

Thread::~Thread()
{
    //quit();

    pthread_attr_destroy(&mPthreadAttr);
}

bool Thread::setThreadFunc(ThreadFunc func, void *para)
{
    if(mIsStarted){
	LOG("set Thread(%s) function fail(thread already started)",
	    mThreadName.c_str());
	return false;
    }

    mThreadFunc = func;
    mPara = para;
    
    return true;
}

bool Thread::start()
{
    int ret;

    if (mIsStarted){
	LOG("%s: start fail(thread already started)",
	    mThreadName.c_str());
	return false;
    }

    if (NULL == mThreadFunc){
	LOG("%s: thread function is NULL", mThreadName.c_str());
	return false;
    }

    ret = pthread_create(&mID, &mPthreadAttr, mThreadFunc,
	mPara);
    if (ret < 0){
	LOG("%s: create thread fail", mThreadName.c_str());
	return false;
    }
    
    mIsStarted = true;
    LOG("thread(%s) start now", mThreadName.c_str());

    return true;
}

bool Thread::join(void **ret)
{
    void *funcRet;

    if (!mIsStarted){
	LOG("thread(%s) is not started", mThreadName.c_str());
	return false;
    }
    
    if (pthread_join(mID, &funcRet) < 0){
	LOG("join thread(%s) fail", mThreadName.c_str());
	return false;
    }

    if (ret != NULL){
	*ret = funcRet;
    }

    mIsStarted = false;
    LOG("join thread(%s) OK", mThreadName.c_str());

    return true;
}

bool Thread::quit()
{
    //尚未实现
    LOG("thread quit is not implement", mThreadName.c_str());

    return false;
}

bool Thread::isStarted()
{
    return mIsStarted;
}

string Thread::getThreadName()
{
    return mThreadName;
}


/*****************************************************/
/*		    private			     */
/*****************************************************/
AtomicInteger Thread::sDefThreadNameIndex;

/*
 * 默认线程名字格式为: Thread-x
 */
void Thread::setDefaultThreadName()
{
    mThreadName = "Thread-";
    mThreadName += sDefThreadNameIndex.incAnd2String();
}

void Thread::setPthreadAttr()
{
    if (pthread_attr_init(&mPthreadAttr)){
	LOG("init thread attr fail");
	return;
    }

    //根据mThreadAttr初始化其它线程属性---待补充
}
