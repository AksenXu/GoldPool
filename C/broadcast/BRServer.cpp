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
int BRServerStart(int listenPort)
{
    sBREpoll = new BREpoll();
    sBRServer = new BRServerListenProcessor(sBREpoll, listenPort);

    LOGD("BRServerStart sBREpoll %p sBRServer %p listenPort %d", sBREpoll, sBRServer, listenPort);
    return 0;
}
