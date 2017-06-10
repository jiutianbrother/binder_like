/*
 * File: Thread.h
 * Description: 线程类
 * Author: fanxiaoyu
 *
 */

#include <string>

#include <ThreadAttr.h>
#include <AtomicInteger.h>


#ifndef	    _THREAD_H
#define	    _THREAD_H

class Thread {
public:
    typedef void *(*ThreadFunc)(void *para);

    Thread(std::string name="", ThreadAttr attr=ThreadAttr());
    ~Thread();

    bool setThreadFunc(ThreadFunc func, void *para);
    bool start();
    bool join(void **ret);
    bool quit();

    bool isStarted();
    std::string getThreadName();

private:
    void setDefaultThreadName();

    void setPthreadAttr();

private:
    static AtomicInteger sDefThreadNameIndex;

private:
    pthread_t mID;

    pthread_attr_t mPthreadAttr;
    ThreadAttr mThreadAttr;

    ThreadFunc mThreadFunc;
    void *mPara;

    std::string mThreadName;
    bool mIsStarted;
};

#endif
