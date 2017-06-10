/*
 * File: Logger.cpp
 * Description: 简易日志系统实现
 * Author: fanxiaoyu
 *
 */

#include <unistd.h>
#include <stdarg.h>

#include <sstream>
#include <iostream>

#include <Logger.h>
#include <Timestamp.h>

using namespace std;

/* 路径分隔符 */
#define		PATH_SEPARATOR		'/'


/****************************************************/
/*		public				    */
/****************************************************/
Logger::Logger(string file)
    : filePath("")
    , mutex(false)
{

    /* 获取日志文件完整路径 */
    if(file.empty())
	filePath = getDefaultLogFile();
    else if(file.find(PATH_SEPARATOR) == string::npos){
	filePath.insert(0, LOGGER_DIR_PREFIX);
	filePath += file;
    }
    else
	filePath = file;

    /* 打开日志文件 */
    ofs.open(filePath.c_str(), ios_base::trunc);
}

void Logger::log(string file, string func, const char* fmt, ...)
{
    char str[MAX_LOG_STR];
    string strTemp;
    va_list ap;

    MutexGuard guard(&mutex);
    
    strTemp += "[";
    strTemp += Timestamp::getNowString();
    strTemp += "][";
    strTemp += file;
    strTemp += "][";
    strTemp += func;
    strTemp += "]";

    va_start(ap, fmt);
    vsnprintf(str, MAX_LOG_STR, fmt, ap);
    va_end(ap);

    strTemp += str;
    ofs << strTemp << endl;
    ofs.flush();
}

/************************************************/
/*	    private				*/
/************************************************/
string Logger::getDefaultLogFile()
{
    string path(LOGGER_DIR_PREFIX);

    ostringstream oss;
    oss << (int)getpid() << LOGGER_FILE_SUFFIX;

    return path + oss.str();
}
