#ifndef __BR_CLIENT_H_H_
#define __BR_CLIENT_H_H_

#include "BRType.h"

int registerBroadcast(int cmd, BRFuncCB cb, void* cookie);
int unregisterBroadcast(int cmd, void* cookie);

int sendBroadcast(int cmd, const char* str, const char* data, int dataSize);

#endif
