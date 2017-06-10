/*
 * File: MutexLock.cpp
 * Description: 互斥锁实现
 * Author: fanxiaoyu
 *
 */

#include <iostream>

#include <errno.h>

#include <MutexLock.h>
#include <Logger.h>

using namespace std;

extern Logger log;

/**************************************************/
/*		public				  */
/**************************************************/
MutexLock::MutexLock(bool isLock, MutexLockAttr attr)
{
    setPthreadMutexAttr();

    if (pthread_mutex_init(&mLock, &mPthreadLockAttr)){
	cerr << "init mutex fail" << endl;
	return;
    }

    if (isLock){
	lock();
    }
}

MutexLock::~MutexLock()
{
    pthread_mutex_destroy(&mLock);

    pthread_mutexattr_destroy(&mPthreadLockAttr);
}

void MutexLock::lock()
{
    if (pthread_mutex_lock(&mLock)){
	cerr << "mutex lock fail" << endl;
    }
}

void MutexLock::unlock()
{
    if (pthread_mutex_unlock(&mLock)){
	cerr << "mutex unlock fail" << endl;
    }
}

bool MutexLock::trylock()
{
    int ret = pthread_mutex_trylock(&mLock);
    
    if (ret < 0 && EBUSY != ret){
	cerr << "mutex trylock fail" << endl;
    }

    return (0 == ret);
}


MutexGuard::MutexGuard(MutexLock *mutex)
    : mLock(mutex)
{
    mLock->lock();
}

MutexGuard::~MutexGuard()
{
    mLock->unlock();
}

/**************************************************/
/*		private				  */	
/**************************************************/
void MutexLock::setPthreadMutexAttr()
{
    //目前只是用默认值
    if (pthread_mutexattr_init(&mPthreadLockAttr)){
	cerr << "init mutex attr fail" << endl;
	return;
    }
    //根据mLockAttr设置相应的锁属性---待补充
}
