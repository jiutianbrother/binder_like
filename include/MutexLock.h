/*
 * File: MutexLock.h
 * Description: 互斥锁
 * Author: fanxiaoyu
 *
 */

#include <pthread.h>

#ifndef	    _MUTEX_LOCK_H
#define	    _MUTEX_LOCK_H

class MutexLock {

public:
    MutexLock(bool isLock = true);
    ~MutexLock();

private:
    pthread_mutex_t mLock;
};

#endif
