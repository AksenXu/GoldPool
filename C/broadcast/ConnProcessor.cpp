#define LOG_TAG "ConnProcessor"

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "Log.h"
#include "ConnProcessor.h"
#include "BREpoll.h"
#include "BRType.h"
#include "SocketUtil.h"

BaseProcessor::BaseProcessor(int fd, BREpoll* poller) 
{
    mFD = fd;
    mPoller = poller;
}

BaseProcessor::~BaseProcessor() 
{
    if(mFD >= 0) {
        close(mFD);
        mFD = -1;
    }
}
int BaseProcessor::readInputData()
{
    if(mFD < 0) {
        LOGE("readInputData failed, mFD %d", mFD);
        return -1;
    }

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
    AutoMutex l(mMutex);
    if(mFD < 0) {
        LOGE("writeOutputData failed, mFD %d", mFD);
        return -1;
    }

    if(mOutputData.size <= 0) {
        LOGW("writeOutputData failed: not enough data mOutputData.size %d", mOutputData.size);
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

BRServerListenProcessor::BRServerListenProcessor(BREpoll* poller, int listenPort) 
            : BaseProcessor(-1, poller)
{
    struct sockaddr_storage addr;
    memset(&addr, 0, sizeof (addr));

    //create Socket and listen
    int fd = socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC, 0);
    if(fd < 0) {
        LOGE("create socket error: %s", strerror(errno));
    } else {    
        struct sockaddr_in*  serverAddr4 = (struct sockaddr_in*)&addr;
        serverAddr4->sin_family = AF_INET;
        serverAddr4->sin_addr.s_addr = htonl(INADDR_ANY);
        serverAddr4->sin_port = htons(listenPort);

        if(bind(fd, (struct sockaddr *)&addr, sizeof(sockaddr_storage)) < 0){
            LOGE("Broadcast Server bind fd %d port %d error: %s", fd, listenPort, strerror(errno));
            close(fd);
        } else if(listen(fd, SOMAXCONN) < 0){
            LOGE("Broadcast Server listen fd %d port %d error: %s", fd, listenPort, strerror(errno));
            close(fd);
        } else {
            LOGD("Broadcast Server listen port %d OK fd %d", listenPort, fd);
            socketBlock(fd, 0);
            socketCloExec(fd);
            mFD = fd;
        }
    }
    LOGD("BRServerListenProcessor %p fd %d poller %p create", this, mFD, mPoller);

    if(mFD != -1) {
        poller->regist(this);
    }
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
    //create from client process, we need connect to server first
    if(fd == -1) {
       connectServer();
    }
    LOGD("BRClientProcessor %p fd %d poller %p create", this, mFD, mPoller);
}

BRClientProcessor::~BRClientProcessor()
{
    LOGD("BRClientProcessor %p fd %d poller %p destroy", this, mFD, mPoller);
}

int BRClientProcessor::connectServer()
{
    struct sockaddr_in serverAdd;
    bzero(&serverAdd, sizeof(serverAdd));
    serverAdd.sin_family = AF_INET;
    serverAdd.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAdd.sin_port = htons(DEFAULT_BROADCAST_SERVER_PORT);

    int fd = socket(AF_INET, SOCK_STREAM, 0);

    int connResult = connect(fd, (struct sockaddr *)&serverAdd, sizeof(serverAdd));
    if (connResult < 0) {
        LOGE("Connect port %d failed %s", DEFAULT_BROADCAST_SERVER_PORT, strerror(errno));
        close(fd);
    } else {
        mFD = fd;
    }
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
        LOGW("mFD %d EPOLLRDHUP event coming, disconnect", mFD);
        return -1;

    }
    return 0;
}

int BRClientProcessor::processData()
{
    //TODO:
    return 0;
}

int BRClientProcessor::appendData(const char* data, int length)
{
    AutoMutex l(mMutex);
    if(mFD == -1) {
       connectServer();
    }
    mOutputData.appendData(data, length);
    return 0;
}