/*
 * File: MutexLock.h
 * Description: 互斥锁
 * Author: fanxiaoyu
 *
 */

#ifndef	    _MUTEX_LOCK_H
#define	    _MUTEX_LOCK_H

#include <pthread.h>

#include <MutexLockAttr.h>

class MutexLock {

public:
    MutexLock(bool isLock = true, MutexLockAttr attr = MutexLockAttr());
    ~MutexLock();

    void lock();
    void unlock();
    bool trylock();

private:
    void setPthreadMutexAttr();

private:
    pthread_mutex_t mLock;

    pthread_mutexattr_t mPthreadLockAttr;
    MutexLockAttr mLockAttr;
};

class MutexGuard {
public:
    MutexGuard(MutexLock *mutex);
    ~MutexGuard();

private:
    MutexLock *mLock;
};

#endif
