#define LOG_TAG "BRClient"
#include "BRClient.h"
#include "ConnProcessor.h"
#include "BRUtil.h"
#include "BRType.h"
#include "BREpoll.h"

static BREpoll* sBREpoll = NULL;
static BRClientProcessor* sBRClient = NULL;


static int buildCmdDataandSend(int cmd, const char* str, const char* data, int dataSize)
{
    
}

int createBRClient()
{
    sBREpoll = new BREpoll();
    sBRClient = new BRClientProcessor(-1, sBREpoll);
    LOGI("createBRClient sBREpoll %p sBRClient %p", sBREpoll, sBRClient);
    return 0;
}

int destroyBRClient()
{
    if(sBRClient) delete sBRClient;
    if(sBREpoll) delete sBREpoll;
    LOGI("destroyBRClient done");
    return 0;
}

int registerBroadcast(const char* cmd, BRFuncCB cb, void* cookie)
{
    return 0;
}

int unregisterBroadcast(const char* cmd, BRFuncCB cb)
{
    return 0;
}

int sendBroadcast(const char* cmd, const char* str, const char* data, int dataSize)
{
    return 0;
}