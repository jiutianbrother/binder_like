/*
 * File: Timestamp.cpp
 * Description: 毫秒级的时间戳
 * Author: fanxiaoyu
 *
 */

#include <stdio.h>

#include <iostream>

#include <Timestamp.h>

using namespace std;


/*****************************************************/
/*		static				     */
/*****************************************************/
struct timeval Timestamp::now()
{
    struct timeval tv;

    gettimeofday(&tv, NULL);

    return tv;
}

string Timestamp::getNowString()
{
    char strTime[256];
    struct tm *p;
    struct timeval tv;

    tv = now();
    p = localtime(&tv.tv_sec);

    sprintf(strTime, "%04d-%02d-%02d-%02d-%02d-%02d-%03d",
	(1900+p->tm_year), (1+p->tm_mon), p->tm_mday,
	p->tm_hour, p->tm_min, p->tm_sec, tv.tv_usec/1000);

    return strTime;
}
