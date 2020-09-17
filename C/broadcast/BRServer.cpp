#define LOG_TAG "BRServer"
#include "BREpoll.h"
#include "ConnProcessor.h"
#include "BRServer.h"
#include "BRUtil.h"
#include "BRType.h"

static BREpoll* sBREpoll = NULL;
static BRServerListenProcessor* sBRServer = NULL;

/*
 * Create Broadcast Server and start Listen/accept client connect
 */
static int BRServerStart(int listenPort = DEFAULT_BROADCAST_SERVER_PORT)
{
    sBREpoll = new BREpoll();
    sBRServer = new BRServerListenProcessor(sBREpoll, listenPort);

    LOGD("BRServerStart sBREpoll %p sBRServer %p listenPort %d", sBREpoll, sBRServer, listenPort);
    return 0;
}

int main()
{
    LOGI("BroadCast Server Started");
    BRServerStart();
    loopAndWait("Server");
    return 0;
}