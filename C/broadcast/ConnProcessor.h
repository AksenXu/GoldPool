#ifndef _CONN_PROCESSOR_H_
#define _CONN_PROCESSOR_H_

#include <stdio.h>
#include <unistd.h>
#include <inttypes.h>
#include "IOBuffer.h"
#include "Mutex.h"

#define  PROCESS_BUFFER_SIZE  4096
class BREpoll;

class BaseProcessor
{
public:
    BaseProcessor(int fd, BREpoll* poller);
    virtual ~BaseProcessor();

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

    Condition mCond;
    Mutex mMutex;
};

class BRServerListenProcessor : public BaseProcessor
{
public:
    BRServerListenProcessor(BREpoll* poller, int listenPort);
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
    
    //receive broadcast and process
    int processData();

    //send broadcast and wait socket sent
    int appendData(const char* data, int length);

private:
    int connectServer();
};

#endif