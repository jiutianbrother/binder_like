/*
 * File: CondVariable.h
 * Desc: 对pthread线程库中条件变量的封装
 * Author: fanxiaoyu
 *
 */

#include <MutexLock.h>
#include <CondVariableAttr.h>

#ifndef		_COND_VARIABLE_H
#define		_COND_VARIABLE_H

class CondVariable
{
public:
    CondVariable(CondVariableAttr attr=CondVariableAttr());
    ~CondVariable();

    bool wait(MutexLock *lock);
    /*
     * @lock: 已经持有的锁
     * @abstime: 要等待的绝对时间
     * @ret: 0-成功；ETIMEOUT-超时；其它-失败
     */
    int timedWait(MutexLock *lock, struct timeval *abstime);
    bool signal();
    bool broadcast();

private:
    bool initPosixCondAttr();

private:
    pthread_cond_t mCond;
    pthread_condattr_t mPosixCondAttr;

    CondVariableAttr mCondAttr;

    bool mIsCondInited;
    bool mIsCondAttrInited;
};

#endif
