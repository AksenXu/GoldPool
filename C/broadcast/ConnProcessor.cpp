#include <sys/types.h>
#include <sys/socket.h>
#include "Log.h"
#include "ConnProcessor.h"
#include "BREpoll.h"

int BaseProcessor::readInputData()
{
    ssize_t size = recv(mFD, mBuffer, PROCESS_BUFFER_SIZE, MSG_DONTWAIT);
    if(size < 0) {
        LOGE("readInputData recv %d failed: %s", mFD, strerror(errno));
        return 0;
    }

    mInputData.appendData(mBuffer, size);
    processData();
    return size;
}

int BaseProcessor::writeOutputData()
{
    if(mOutputData.size <= 0) {
        LOGW("writeOutputData %d failed: not enough data", mFD);
        return 0;
    }

    ssize_t size = send(mFD, mOutputData.data, mOutputData.size, MSG_DONTWAIT);
    if(size < 0) {
        LOGE("writeOutputData send %d failed: %s", mFD, strerror(errno));
        return 0;
    }

    mOutputData.erase(0, size);
    return size;
}

BRServerListenProcessor::BRServerListenProcessor(int fd, BREpoll* poller) 
            : BaseProcessor(fd, poller)
{
    LOGD("BRServerListenProcessor %p fd %d poller %p create", this, mFD, mPoller);
}

BRServerListenProcessor::~BRServerListenProcessor()
{
    LOGD("BRServerListenProcessor %p fd %d poller %p destroy", this, mFD, mPoller);
}

int BRServerListenProcessor::processEvent(uint32_t event)
{
    if(event & EPOLLIN) {
        //receive connect, accept
        int connectfd = accept(mFD, NULL, NULL);
        if(connectfd < 0) {
            LOGE("accept error! %s\n", strerror(errno));
            return 0;
        }

        BRClientProcessor* client = new BRClientProcessor(connectfd, mPoller);
        mPoller->regist(client);
    } else {
        LOGE("BRServerListenProcessor should not receive this event 0x%x", event);
    }

    return 0;
}

BRClientProcessor::BRClientProcessor(int fd, BREpoll* poller) 
                : BaseProcessor(fd, poller) 
{
    LOGD("BRClientProcessor %p fd %d poller %p create", this, mFD, mPoller);
}

BRClientProcessor::~BRClientProcessor()
{
    LOGD("BRClientProcessor %p fd %d poller %p destroy", this, mFD, mPoller);
}

int BRClientProcessor::processEvent(uint32_t event)
{
    if(event & EPOLLIN) {
        readInputData();
    }
        
    if(event & EPOLLOUT) {
        writeOutputData();
    }

    if(event & EPOLLRDHUP) {
        //socket closed
        return -1;

    }
    return 0;
}

int BRClientProcessor::processData()
{
    return 0;
}
