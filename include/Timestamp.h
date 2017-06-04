#ifndef	    _TIMESTAMP_H
#define	    _TIMESTAMP_H

#include <string>

#include <sys/time.h>
#include <stdlib.h>

class Timestamp
{
public:
    static struct timeval now();
    static std::string getNowString();

    Timestamp(struct timeval val=Timestamp::now()){
	timeValue = val;
    }

private:
    struct timeval timeValue;
};

#endif
