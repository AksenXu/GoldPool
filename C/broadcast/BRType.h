#ifndef _BT_TYPE_H_H_
#define _BT_TYPE_H_H_

#include "Log.h"

#define     BR_REGISTER     "BR_REGISTER"
#define     BR_UNREGISTER   "BR_UNREGISTER"
#define     BR_SEND         "BR_SEND"

#define     BR_TEST         "BR_TEST"

struct BRCmd {
    int syncCode;           //Fixed synccode: 0x83 0x09 0x11 0x05
    int totalLen;           //total length include syncCode until next syncCode
    int cmdLen;             //cmd length
    const char* cmd;        //broadcast command, such as register/send ... without '\0'
                            //in libs, use BRCMD enum, for user define command, it can be defined outside
    int strLen;             //string length
    const char* strParam;   //Json format
    int dataLen;            //binary data length
    const char* dataParam;  //binary data
};

typedef int (* BRFuncCB)(BRCmd& cmd, void* cookie);

#define DEFAULT_BROADCAST_SERVER_PORT 10961

#endif