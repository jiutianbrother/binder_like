#include <string>
#include <fstream>

#include <MutexLock.h>

#ifndef	    __LOGGER_H
#define	    __LOGGER_H

/* 默认日志文件所在目录 */
#define	    LOGGER_DIR_PREFIX	    "/tmp/binder_like/log/"
/* 默认日志文件后缀 */
#define	    LOGGER_FILE_SUFFIX	    ".log"


#ifdef DEBUG
#define	    LOG(fmt, args...)	    do{\
					log.log(__FILE__, __func__, fmt, ##args);\
				    }while(0)
#else
#define	    LOG(pLog, fmt, args...)
#endif


/* 一条日志的最大长度 */
#define	    MAX_LOG_STR		    512

class Logger{

public:
    Logger(std::string file=std::string(""));
    ~Logger(){
	ofs.close();
    }

    void log(std::string file, std::string func, const char* fmt, ...);

    /*Just for test*/
    std::string getLogFilePath(){
	return filePath;
    }
    /*public for test*/
    std::string getDefaultLogFile();

private:
    std::string filePath;
    std::ofstream ofs;

    /*线程安全*/
    MutexLock mutex;
};

#endif
