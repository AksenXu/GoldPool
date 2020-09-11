#ifndef _SOCKET_UTIL_H_
#define _SOCKET_UTIL_H_
#include <unistd.h>
#include <fcntl.h>

static inline int socketBlock(int sockFd, int block) 
{
    int flags = fcntl(sockFd, F_GETFL, 0);
    flags = block ? (flags&~O_NONBLOCK) : (flags|O_NONBLOCK);
    return fcntl(sockFd, F_SETFL, flags);
}

static inline int socketCloExec(int sockFd)
{
    int flags = fcntl(sockFd, F_GETFL, 0);
    flags = flags|FD_CLOEXEC;
    return fcntl(sockFd, F_SETFL, flags);	
}

#endif