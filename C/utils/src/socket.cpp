#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "socket.h"

int sendUDP(const char * str, const char* ip, const int port)
{
    int sockfd = socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd < 0) {
        return errno;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);
    
    void* ptr = &addr;
	int n = sendto(sockfd, str, strlen(str), 0, (const struct sockaddr*)ptr, sizeof(struct sockaddr_in));
    if(n > 0){
        n = 0; //OK
    }else {
        n = errno;
    }

	close(sockfd);
    return n;
}
