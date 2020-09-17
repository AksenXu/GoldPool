#define LOG_TAG "BREpoll"

#include "Log.h"
#include "ConnProcessor.h"
#include "listhead.h"
#include "BREpoll.h"

struct processorList {
    list_head list;
    struct BaseProcessor* processor;
};

BREpoll::BREpoll()
{
    createEpoll();

    mExit = 0;
    init_list_head2(&mRegistProcessorsHeadList);
    pthread_mutex_init(&mMutex, NULL);
    pthread_cond_init(&mCond, NULL);
    pthread_create(&mThread, NULL, BREpoll::BREpollThread, this);   

    LOGD("BREpoll %p mThread 0x%lx create", this, mThread); 
}

BREpoll::~BREpoll()
{
    LOGD("BREpoll %p mThread 0x%lx destroy", this, mThread); 

    if(mThread != -1) {
        //how to wakeup the mThread, and exit?
        //close一个正在wait的epoll句柄会导致未知的错误,所以不可以直接close
        //如果epoll又长时间没有事件，那就会导致这个线程退不出去卡住
        //将epoll设置为一个timeout的方式
        pthread_mutex_lock(&mMutex);
        mExit = 1;
        pthread_mutex_unlock(&mMutex);

        pthread_join(mThread, NULL);
        mThread = -1;
    }

    pthread_cond_destroy(&mCond);
    pthread_mutex_destroy(&mMutex);

    destroyEpoll();
    LOGD("BREpoll %p destroy done", this); 
}

struct processorList* BREpoll::findProcessListLocked(int fd)
{
    struct list_head *pos, *n;
    list_for_each_safe(pos, n, &mRegistProcessorsHeadList) {
        processorList* l = list_entry(pos, processorList, list);
        if(fd == l->processor->getFD()) {
            return l;
        }
    }

    LOGI("BREpoll new fd %d coming, add to epoll", fd);
    return NULL;
}

int BREpoll::_epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)
{
    int ret = epoll_ctl(epfd, op, fd, event);
    if(ret != 0) {
        LOGE("BREpoll epoll_ctl %d op %x fd %d failed %s", epfd, op, fd, strerror(errno));
        return -1;
    }

    return 0;
}

int BREpoll::regist(struct BaseProcessor* processor, int events)
{
    int fd = processor->getFD();
    if(fd < 0) {
        LOGE("BREpoll regist fd %d failed", fd);
        return -1;
    } 
    pthread_mutex_lock(&mMutex);

    struct epoll_event epEvent;
    if(events == 0) {
        events = EPOLLIN | EPOLLRDHUP;
    }
    epEvent.events = events;
    epEvent.data.ptr = processor;

    struct processorList* l = findProcessListLocked(fd);
    if(l == NULL) {
        l = new processorList;
        l->processor = processor;
        _epoll_ctl(mEpollFD, EPOLL_CTL_ADD, fd, &epEvent);
        list_add_tail(&l->list, &mRegistProcessorsHeadList);
        mRegistProcessorsHeadList.count ++;
    } else {
        _epoll_ctl(mEpollFD, EPOLL_CTL_MOD, fd, &epEvent);
    }

    LOGI("BREpoll regist fd %d event 0x%x EPOLLIN %x EPOLLRDHUP %x EPOLLOUT %x epolllistcount %d", 
                        fd, events, EPOLLIN, EPOLLRDHUP, EPOLLOUT, mRegistProcessorsHeadList.count);
    pthread_mutex_unlock(&mMutex);
}

int BREpoll::unregist(struct BaseProcessor* processor)
{
    pthread_mutex_lock(&mMutex);

    struct epoll_event event;
    int fd = processor->getFD();
    struct processorList* l = findProcessListLocked(fd);
    if(l) {
        list_del_init(&l->list);
        mRegistProcessorsHeadList.count --;
        delete l;
        _epoll_ctl(mEpollFD, EPOLL_CTL_DEL, fd, &event);
        LOGI("BREpoll unregist fd %d", fd);
    } else {
        LOGE("BREpoll unregist fd %d failed, can't find record", fd);
    }

    pthread_mutex_unlock(&mMutex);
}

void* BREpoll::BREpollThread(void * brepoll)
{
    BREpoll* ep = (BREpoll*) brepoll;
    ep->loopThread();
    return NULL;
}

int BREpoll::loopThread()
{
    LOGD("BREpoll loopThread start");
    while(true) {
        pthread_mutex_lock(&mMutex);
        if(mExit) {
            pthread_mutex_unlock(&mMutex);
            break;
        }
        pthread_mutex_unlock(&mMutex);

        struct epoll_event events[EPOLL_WAIT_COUNT];
        int count = epoll_wait(mEpollFD, events, EPOLL_WAIT_COUNT, EPOLL_WAIT_TIMEOUT_MS);
        if(count < 0) {
            LOGE("loopThread epoll_wait failed %s", strerror(errno));
            continue;
        }

        for(int i = 0; i < count; i++) {
            struct BaseProcessor* processor = (struct BaseProcessor*)events[i].data.ptr;
            int ret = processor->processEvent(events[i].events);
            if(ret == -1) {
                unregist(processor);
                delete processor;
            }
        }
    }

    LOGD("BREpoll loopThread exit");
    return 0;
}

int BREpoll::createEpoll()
{
    //this 1 is meaningless when new kernels, kernel will dynamic alloc memory
    mEpollFD = epoll_create(1);
    if(mEpollFD < 0) {
        LOGE("epoll_create failed: %s", strerror(errno));
        return -1;
    }
    LOGD("epoll_create mEpollFD = %d", mEpollFD);
    return 0;
}

int BREpoll::destroyEpoll()
{
    if(mEpollFD != -1) {
        close(mEpollFD);
        LOGD("close mEpollFD = %d", mEpollFD);

        mEpollFD = -1;
    }
}