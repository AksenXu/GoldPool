#ifndef _BT_TYPE_H_H_
#define _BT_TYPE_H_H_

enum BRCMD
{
    BR_REGISTER = 0,
    BR_SEND = 1,

    BR_TEST = 100,
};

struct BRCmd {
    int syncCode;           //Fixed synccode: 0x83 0x09 0x11 0x05
    int cmdLen;             //total length include syncCode until next syncCode
    int cmdid;              //broadcast command, such as register/send ...
                            //in libs, use BRCMD enum, for user define command, it can be defined outside
    const char* strParam;   //Json format
    int dataLen;            //binary data length
    const char* dataParam;  //binary data
};

typedef int (* BRFuncCB)(BRCmd& cmd, void* cookie);

#define DEFAULT_BROADCAST_SERVER_PORT 10961

#endif