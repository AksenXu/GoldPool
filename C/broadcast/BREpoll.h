#ifndef __BROADCAST_EPOLL_H_
#define __BROADCAST_EPOLL_H_

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/epoll.h>
#include "listhead.h"
#include "Mutex.h"

struct processorList;
class BaseProcessor;

class BREpoll {
public:
    BREpoll();
    virtual ~BREpoll();

    /* 
    * reg/unreg epoll processor include listen fd and connect fd
    */
    int regist(class BaseProcessor* prococessor, int events = 0);
    int unregist(class BaseProcessor* prococessor);

    static void * BREpollThread(void * brepoll);
private:
    /* 
    * Create/Destroy epoll to wait epoll event
    */
    int createEpoll(); 
    int destroyEpoll();

    int loopThread();

    struct processorList* findProcessListLocked(int fd);
    int _epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);

    int mEpollFD;
    int mExit;

    pthread_t mThread;
    pthread_cond_t mCond;
    pthread_mutex_t mMutex;

    list_head2 mRegistProcessorsHeadList;
};

#define EPOLL_WAIT_COUNT        32
#define EPOLL_WAIT_TIMEOUT_MS   1000

#endif