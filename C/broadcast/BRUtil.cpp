#include "BRUtil.h"

void loopAndWait(const char* user)
{
    static int time = 0;
    while(1) {
        time ++;
        usleep(1000000);
        LOGD("BroadCast %s Process Run %d seconds", user, time);
    }
}
