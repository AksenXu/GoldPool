#ifndef __SOCKET_UTIL_H_
#define __SOCKET_UTIL_H_

#include <sys/types.h>
#define ANDROID_SOCKET_DIR		"/dev/socket"

int create_socket(const char *name, int type, mode_t perm,
                  uid_t uid, gid_t gid);

#endif //__SOCKET_UTIL_H_
