/*
 * File: UnixStreamSocket.cpp
 * Description: Unix域流套接字
 * Author: fanxiaoyu
 *
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <sys/un.h>
#include <errno.h>

#include <string>

#include <UnixStreamSocket.h>
#include <Logger.h>


using namespace std;


extern Logger log;

/*******************************************************/
/*		    public			       */
/*******************************************************/

UnixStreamSocket::UnixStreamSocket(string addr, SocketType type)
    : mFd(-1)
    , mAddr(addr)
    , mState(UN_INITED)
    , mSockType(type)
{ 
    if(COMMON == type)
	return;

    if(false == init())
	return;
    
    switch(mSockType){
    case SERVER:
	bind(addr);
	break;
    case CLIENT:
	connect(addr);
	break;
    default:
	break;
    }
}

UnixStreamSocket::~UnixStreamSocket()
{
    if(UN_INITED != mState){
	close(mFd);
	mFd = -1;
    }
}

bool UnixStreamSocket::bind(string addr)
{
    int ret;
    struct sockaddr_un addr1;

    if(addr.empty()){
	LOG("addr is empty");
	return false;
    }
    if(INITED != mState){
	LOG("invlaid socket state(current state:%d)", mState);
	return false;
    }

    unlink(addr.c_str());

    bzero(&addr1, sizeof(addr1));
    addr1.sun_family = AF_LOCAL;
    strncpy(addr1.sun_path, addr.c_str(), sizeof(addr1.sun_path)-1);
    ret = ::bind(mFd, (struct sockaddr*)&addr1, sizeof(addr1));
    if(ret < 0){
	LOG("bind to %s failed", addr1.sun_path);
	return false;
    }

    mSockType = SERVER;
    mAddr = addr;
    mState = BINDED;
    
    LOG("bind to socket:%d success", addr1.sun_path);

    return true;
}

bool UnixStreamSocket::listen(int backlog)
{
    if(BINDED != mState){
	LOG("invlaid socket state(current state:%d)", mState);
	return false;
    }

    if(backlog < 0){
	LOG("invalied backlog:%d, use default:4", backlog);
	backlog = 4;
    }
    
    if(::listen(mFd, backlog) < 0){
	LOG("listen failed");
	return false;
    }

    return true;
}

UnixStreamSocket* UnixStreamSocket::accept()
{
    struct sockaddr_un addr;
    UnixStreamSocket *sock;
    int fd;
    socklen_t addrlen;

    if(LISTENED != mState){
	LOG("invalid socket state:%d", mState);
	return NULL;
    }

    while(true){
	addrlen = sizeof(addr);
	fd = ::accept(mFd, (struct sockaddr *)&addr, &addrlen);
	if(fd < 0 && errno == EINTR)
	    continue;
	else if(fd < 0){
	    LOG("accept failed: %d", errno);
	}
	break;
    }

    if(fd < 0)
	return NULL;

    sock = new UnixStreamSocket(addr.sun_path);
    sock->mState = CONNECTED;
    sock->mFd = fd;
    
    return sock;
}

bool UnixStreamSocket::connect(string addr)
{
    int ret;
    struct sockaddr_un addr1;

    if(addr.empty()){
	LOG("addr is empty");
	return false;
    }
    if(INITED != mState){
	LOG("invlaid socket state(current state:%d)", mState);
	return false;
    }
 
    bzero(&addr1, sizeof(addr1));
    addr1.sun_family = AF_LOCAL;
    strncpy(addr1.sun_path, addr.c_str(), sizeof(addr1.sun_path)-1);
    ret = ::connect(mFd, (struct sockaddr*)&addr1, sizeof(addr1));
    if(ret < 0){
	LOG("connect to %s failed", addr1.sun_path);
	return false;
    }

    mSockType = CLIENT;
    mAddr = addr;
    mState = CONNECTED;

    LOG("connect to socket: %s success", addr1.sun_path);

    return true;
}

/*******************************************************/
/*		    private			       */
/*******************************************************/

bool UnixStreamSocket::init()
{
    mFd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if(mFd < 0){
	mFd = -1;
	LOG("init socket failed");
	return false;
    }
    mState = INITED;
    return true;
}
