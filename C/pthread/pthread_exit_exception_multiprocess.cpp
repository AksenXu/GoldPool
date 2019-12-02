/*================================================================
 *   Copyright (C) 2019 Ltd. All rights reserved.
 *   
 *   File Name ：robust_mutex.c
 *   Author    ：Hamilton
 *   Date      ：2019-07-30
 *   Descriptor：
 *
 ================================================================*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define SHM_NAME    "fasdfasfasfas"


#define handle_error_en(en, msg) \
    do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

static pthread_mutex_t *mtx;
static int fd_shm;

void shm_mutex_init(pthread_mutex_t  **mutex)
{ 
    pthread_mutexattr_t attr;

    pthread_mutexattr_init(&attr);
    /* initialize the attributes object */
    pthread_mutexattr_setrobust(&attr, PTHREAD_MUTEX_ROBUST);
    /* set robustness */

    // Get shared memory
    if ((fd_shm = shm_open (SHM_NAME, O_RDWR | O_CREAT, 0660)) == -1)
        perror ("shm_open");

    if (ftruncate (fd_shm, sizeof (pthread_mutex_t)) == -1)
        perror ("ftruncate");

    if ((*mutex = (pthread_mutex_t  *)mmap (NULL, sizeof (pthread_mutex_t), PROT_READ | PROT_WRITE, MAP_SHARED,
                    fd_shm, 0)) == MAP_FAILED)
        perror ("mmap");

    pthread_mutex_init(*mutex, &attr);   /* initialize the mutex */
}
int main(int argc, char *argv[])
{
    int s;

    shm_mutex_init(&mtx);

    if ((s = fork()) < 0)
    {
        perror("fork.");    
    }
    else if (s == 0) // child
    {
        sleep(1);
            printf("\n[bad owner] Setting lock...\n");
            pthread_mutex_lock(mtx);
            printf("[bad owner] Locked. Now exiting without unlocking.\n");
    }
    else
    {
        int i = 5;
        while(i--)
        {
            s = pthread_mutex_lock(mtx);

            if (s == EOWNERDEAD) 
            {
                printf("\n[main thread] pthread_mutex_lock() returned EOWNERDEAD\n");

                printf("[main thread] Now make the mutex consistent\n");
                s = pthread_mutex_consistent(mtx);
                if (s != 0)
                    handle_error_en(s, "pthread_mutex_consistent");

                printf("[main thread] Mutex is now consistent; unlocking\n");
                s = pthread_mutex_unlock(mtx);
                if (s != 0)
                    handle_error_en(s, "pthread_mutex_unlock");

            } 
            else if (s < 0)
            {
                printf("\n[main thread] pthread_mutex_lock() unexpectedly failed\n");
                handle_error_en(s, "pthread_mutex_lock");
            }
            else 
            {
                printf("\n[main thread] pthread_mutex_lock success.\n");
                printf("do somthing.... \n");
                s = pthread_mutex_unlock(mtx);
                if (s != 0)
                    handle_error_en(s, "pthread_mutex_unlock");
            }

            sleep(1);
        }
    }
    exit(EXIT_SUCCESS);
}