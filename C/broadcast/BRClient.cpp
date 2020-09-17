#define LOG_TAG "BRClient"
#include "BRClient.h"
#include "ConnProcessor.h"
#include "BRUtil.h"
#include "BRType.h"
#include "BREpoll.h"

static BREpoll* sBREpoll = NULL;
static BRClientProcessor* sBRClient = NULL;

static int createBRClient()
{
    sBREpoll = new BREpoll();
    sBRClient = new BRClientProcessor(-1, sBREpoll);
    return 0;
}

static int destroyBRClient()
{
    if(sBRClient) delete sBRClient;
    if(sBREpoll) delete sBREpoll;
    LOGI("destroyBRClient done");
}

static int buildCmdDataandSend(int cmd, const char* str, const char* data, int dataSize)
{
    
}

int registerBroadcast(int cmd, BRFuncCB cb, void* cookie)
{
    if(sBREpoll == NULL) {
        createBRClient();
    }



    return 0;
}

int unregisterBroadcast(int cmd, void* cookie)
{
    if(sBREpoll == NULL) {
        createBRClient();
    }

    return 0;
}

int sendBroadcast(int cmd, const char* str, const char* data, int dataSize)
{
    if(sBREpoll == NULL) {
        createBRClient();
    }

    return 0;
}

static int testBRFuncCB(BRCmd& cmd, void* cookie)
{
    LOGI("testBRFuncCB cmd %d BR_TEST %d cookie %p", cmd.cmdid, BR_TEST, cookie);
}

int main(int argc, char* argv[])
{
    if(argc == 1) {
        LOGI("create client and wait broadcast");
        registerBroadcast(BR_TEST, testBRFuncCB, NULL);
        loopAndWait("client");
    } else {
        sendBroadcast(BR_TEST, NULL, NULL, 0);
    }

    destroyBRClient();
    return 0;
}