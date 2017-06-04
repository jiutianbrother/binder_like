/*
 * File: UnixStreamSocket.h
 * Description: Unix域流套接字
 * Author: fanxiaoyu
 *
 */

#include <string>

#ifndef	    _UNIX_STREAM_SOCKET_H
#define	    _UNIX_STREAM_SOCKET_H

class UnixStreamSocket
{
public:
    enum SocketType {
	SERVER = 0,
	CLIENT = 1,
	COMMON = 2	/* 普通的通信套接字 */
    };

    UnixStreamSocket(std::string addr = "", SocketType type=COMMON);
    ~UnixStreamSocket();

    bool bind(std::string addr);
    bool listen(int backlog = 4);
    /*
     * 返回的通信套接字是动态分配的
     */
    UnixStreamSocket* accept();

    bool connect(std::string addr);

private:
    /* 等价于socket() */
    bool init();

private:
    enum State {
	UN_INITED = 0,
	INITED = 1,
	BINDED = 2,
	LISTENED = 3,
	CONNECTED = 4,
    }mState;

    int mFd;

    std::string mAddr;

    SocketType mSockType;
};

#endif
