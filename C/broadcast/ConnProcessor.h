#ifndef _CONN_PROCESSOR_H_
#define _CONN_PROCESSOR_H_

#include <stdio.h>
#include <unistd.h>
#include <inttypes.h>
#include "IOBuffer.h"

#define  PROCESS_BUFFER_SIZE  4096
class BREpoll;

class BaseProcessor
{
public:
    BaseProcessor(int fd, BREpoll* poller) {
        mFD = fd;
        mPoller = poller;
    }

    virtual ~BaseProcessor() {
        if(mFD >= 0) {
            close(mFD);
            mFD = -1;
        }
    }

    virtual int processEvent(uint32_t event) = 0;
    virtual int processData() { return 0; }
    int getFD() { return mFD; }

    int readInputData();
    int writeOutputData();

protected:
    int mFD;
    char mBuffer[PROCESS_BUFFER_SIZE];//middle buffer
    IOBuffer mInputData;
    IOBuffer mOutputData;
    BREpoll* mPoller;
};

class BRServerListenProcessor : public BaseProcessor
{
public:
    BRServerListenProcessor(int fd, BREpoll* poller);
    virtual ~BRServerListenProcessor();

    int processEvent(uint32_t event);

private:
};

class BRClientProcessor : public BaseProcessor
{
public:
    BRClientProcessor(int fd, BREpoll* poller);
    virtual ~BRClientProcessor();

    int processEvent(uint32_t event);
    int processData();

private:
};

#endif