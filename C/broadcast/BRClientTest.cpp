#include "BRClient.h"
#include "BRUtil.h"

static int testBRFuncCB(BRCmd& cmd, void* cookie)
{
    LOGI("testBRFuncCB cmd %s BR_TEST %s cookie %p", cmd.cmd, BR_TEST, cookie);
}

int main(int argc, char* argv[])
{
    createBRClient();

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