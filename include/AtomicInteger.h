/*
 * File: AtomicInteger.h
 * Description: 原子整数
 * Author: fanxiaoyu
 *
 */

#include <string>
#include <stdio.h>

#include <MutexLock.h>

#ifndef	    _ATOMIC_INTEGER_H
#define	    _ATOMIC_INTEGER_H

class AtomicInteger {
public:
    AtomicInteger(int value = 0)
	: mValue(value)
	, mLock(false)
    {
    }

    ~AtomicInteger()
    {
    }

    int increase(int value = 1)
    {
	MutexGuard guard(&mLock);
	mValue += value;

	return mValue;
    }

    int decrease(int value = 1)
    {
	MutexGuard guard(&mLock);
	mValue -= value;
	
	return mValue;
    }

    std::string incAnd2String(int value = 1)
    {
	char str[20];

	MutexGuard guard(&mLock);
	mValue += value;
	snprintf(str, 20, "%d", mValue);

	return std::string(str);
    }

    std::string decAnd2String(int value = 1)
    {
	char str[20];

	MutexGuard guard(&mLock);
	mValue -= value;
	snprintf(str, 20, "%d", mValue);

	return std::string(str);
    }

private:
    int mValue;
    MutexLock mLock;
};

#endif
