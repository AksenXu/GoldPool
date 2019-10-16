#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "backtrace.h"

extern void dump_backtrace(pid_t tid);

int main(int argc, char* argv[])
{
    if(argc < 2) {
        printf("Usage: %s pid\n", argv[0]);
        return 0;
    }
    
    int pid = atoi(argv[1]);
    dump_backtrace(pid);
    return 0;
}
