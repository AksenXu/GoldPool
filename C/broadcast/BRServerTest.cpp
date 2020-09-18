#define LOG_TAG "BRServerTest"
#include "BRType.h"
#include "BRServer.h"
#include "BRUtil.h"

int main()
{
    LOGI("BroadCast Server Started");
    BRServerStart();
    loopAndWait("Server");
    return 0;
}