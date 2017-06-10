/*
 * File: CondVariable.cpp
 * Desc: 条件变量实现文件
 * Author: fanxiaoyu
 *
 */

#include <errno.h>

#include <Logger.h>
#include <CondVariable.h>

extern Logger log;

/***********************************************************/
/*		    public				   */
/***********************************************************/
CondVariable::CondVariable(CondVariableAttr attr)
    : mCondAttr(attr)
    , mIsCondInited(false)
    , mIsCondAttrInited(false)
{
    if (false == initPosixCondAttr()){
	return;
    }

    if (pthread_cond_init(&mCond, &mPosixCondAttr) < 0){
	LOG("init cond failed");
	return;
    }
    mIsCondInited = true;
}

CondVariable::~CondVariable()
{
    if (mIsCondAttrInited){
	pthread_condattr_destroy(&mPosixCondAttr);
    }

    if (mIsCondInited){
	pthread_cond_destroy(&mCond);
    }
}

bool CondVariable::wait(MutexLock *lock)
{
    int ret;

    if (!mIsCondInited || !mIsCondAttrInited){
	LOG("mIsCondInited=%d, mIsCondAttrInited=%d",
	    mIsCondInited, mIsCondAttrInited);
	return false;
    }

    ret = pthread_cond_wait(&mCond, &lock->mLock);
    if (ret < 0){
	LOG("cond wait failed(ret=%d)", ret);
	return false;
    }

    return true;
}

int CondVariable::timedWait(MutexLock *lock, struct timeval *abstime)
{
    int ret;
    struct timespec ts;

    if (!mIsCondInited || !mIsCondAttrInited){
	LOG("mIsCondInited=%d, mIsCondAttrInited=%d",
	    mIsCondInited, mIsCondAttrInited);
	return false;
    }

    ts.tv_sec = abstime->tv_sec;
    ts.tv_nsec = abstime->tv_usec * 1000;
    ret = pthread_cond_timedwait(&mCond, &lock->mLock, &ts);
    if (ret < 0 && ret != ETIMEDOUT){
	LOG("cond timed wait failed(ret=%d)", ret);
	return false;
    }
    else if (ret < 0){
	LOG("cond timed out");
    }

    return ret;
}

bool CondVariable::signal()
{
    int ret;

    if (!mIsCondInited || !mIsCondAttrInited){
	LOG("mIsCondInited=%d, mIsCondAttrInited=%d",
	    mIsCondInited, mIsCondAttrInited);
	return false;
    }
    
    ret = pthread_cond_signal(&mCond);
    if (ret < 0){
	LOG("cond signal failed(ret=%d)", ret);
	return false;
    }

    return true;
}

bool CondVariable::broadcast()
{
    int ret;

    if (!mIsCondInited || !mIsCondAttrInited){
	LOG("mIsCondInited=%d, mIsCondAttrInited=%d",
	    mIsCondInited, mIsCondAttrInited);
	return false;
    }
    
    ret = pthread_cond_broadcast(&mCond);
    if (ret < 0){
	LOG("cond broadcast failed(ret=%d)", ret);
	return false;
    }

    return true;
}

/***********************************************************/
/*		    private				   */
/***********************************************************/
bool CondVariable::initPosixCondAttr()
{
    /*目前仅使用默认的条件变量属性*/
    if (pthread_condattr_init(&mPosixCondAttr) < 0){
	LOG("init cond attr failed");
	mIsCondAttrInited = false;
	return false;
    }
    
    mIsCondAttrInited = true;
    return true;
}
