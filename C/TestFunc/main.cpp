#include <stdio.h>
#include <fcntl.h>           /* Definition of AT_* constants */
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>

#if 0
int main(int argc, char* argv[])
{
    
    //int ret = unlink("./testunlink");
    //printf("ret = %d %d/%s\n", ret, errno, strerror(errno));
    
    time_t now_t = time(NULL);
    struct tm now; 

    localtime_r(&now_t, &now);
    printf("now_t %ld => %04d-%02d-%02d %02d:%02d:%02d\n", now_t, (1900+now.tm_year), (1+now.tm_mon), now.tm_mday,
		                            now.tm_hour, now.tm_min, now.tm_sec);
                                    
    now_t = mktime(&now);
    
    printf("now_t after localtime is %ld\n", now_t);
    
    return 0;
}

#endif

void getTimeZone()
{
    struct tm localTime;
	time_t unixTime;
	time(&unixTime);
    localtime_r(&unixTime, &localTime);

    int diff = localTime.tm_gmtoff;
    int hour_diff = abs(diff / 60 / 60);
    int min_diff = abs(diff / 60 % 60);
    // int sec_diff = abs(diff % 60);

    printf("timezone : %c%02d:%02d\n", diff > 0 ? '+' : '-', hour_diff, min_diff);
}

int main()
{
    getTimeZone();
    return 0;
}