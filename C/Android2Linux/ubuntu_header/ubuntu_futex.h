#ifndef __UBUNTU_H__
#define __UBUNTU_H__
#include <unistd.h>
#include <linux/futex.h>
#include <sys/syscall.h>

inline void __futex_wait(volatile void* ftx, int val, const struct timespec *timeout)
{
    syscall(__NR_futex, ftx, FUTEX_WAIT, val, timeout);
}

inline void __futex_wake(volatile void *ftx, int count)
{
    syscall(__NR_futex, ftx, FUTEX_WAKE, count, NULL);
}

#endif