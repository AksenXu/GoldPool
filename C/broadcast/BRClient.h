#ifndef __BR_CLIENT_H_H_
#define __BR_CLIENT_H_H_

#include "BRType.h"

/* @Function: 
 *      createBRClient
 * @Discription: 
 *      first call function in the broadcast client process,
 *      it will create socket connect to server, the broadcast cmd will be send/recv 
 *      through the same socket in one client process.
 * @return:
 *      always return 0 now
 */
int createBRClient();

/* @Function: 
 *      destroyBRClient
 * @Discription: 
 *      last call function in the broadcast client process,
 *      no broadcast will be send/recv after this function
 *      normally, this function will not be called
 * @return:
 *      always return 0 now
 */
int destroyBRClient();

/* @Function: 
 *      registerBroadcast
 * @Discription: 
 *      client register cmd, it will do:
 * 
 * 1 register this cb/cookie in client process cblist
 * 2 register this client process to server process with cmd
 * 3 if cmd broadcast happen, server will notify client process, 
 * client need check the cb/cookie and callback
 * 
 * @Param:
 *      cmd :   broadcast cmd, it MUST be different for every broadcast
 *      cb :    callback function when cmd broadcast happen
 *      cookie: cookie send in cb function
 * @return:
 *      0: success -1: failed
 */
int registerBroadcast(const char* cmd, BRFuncCB cb, void* cookie);

/* @Function: 
 *      unregisterBroadcast
 * @Discription: 
 *      client unregister cmd, it will do:
 * 
 * 1 check the cblist and delete the cb/cookie
 * 2 check the cblist, if no cb/cookie with cmd exist, unregister to server
 * 
 * @Param:
 *      cmd :   broadcast cmd, it MUST be different for every broadcast
 *      cb:     for delete the cb/cookie in cblist
 * 
 * @return:
 *      0: success -1: failed
 */
int unregisterBroadcast(const char* cmd, BRFuncCB cb);

/* @Function: 
 *      sendBroadcast
 * @Discription: 
 *      send broadcast to server
 * 
 * @Param:
 *      cmd:    broadcast cmd
 *      str:    string param, json format
 *      data:   data param, binary 
 *      dataSize:   binary length of data param
 * 
 * @return:
 *      0: success -1: failed
 * 
 * Note: sendBroadcast only append the cmd structure to send buffer, not send out in async mode
 * so after this function, server maybe have not receive the broadcast
 * but in BRClient, we MUST confirm the broadcast will be sent out in later moment.
 */
int sendBroadcast(const char* cmd, const char* str, const char* data, int dataSize);

#endif  //__BR_CLIENT_H_H_
