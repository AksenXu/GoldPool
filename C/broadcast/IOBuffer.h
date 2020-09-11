#ifndef _IOBUFFER_H_INCLUDED
#define _IOBUFFER_H_INCLUDED   1

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "Log.h"

struct IOBuffer {
    char * data;
    int    size;
    int    capacity;    //capacity, always >= size
    int    reserveRound;

    IOBuffer() { 
        init();
    }

    ~IOBuffer() {
        deinit(); 
    }

    void init() {
        data = NULL;
        size = 0;
        capacity = 0;
        reserveRound = 4096;
    }

    void deinit() {
        free(data);
        data = NULL;
        size = 0;
        capacity = 0;    
    }

    int appendData(const void *data, int count){
        int ret = reserve(this->size + count);
        if( ret == 0){
            memcpy( this->data + this->size, data, count);
            this->size += count;
            this->data[this->size] = '\0';
        }
        return ret;
    }

    int reserve(int cap)
    {
        if ( cap > this->capacity) {
            // round up
            const int round = reserveRound;
            int alloc = (cap + 1 + round) & (~(round - 1)); //extra 1 byte for null-terminated char

            char * newData = (char*) realloc(this->data, alloc);
            if(!newData) {
                LOGW("can't allcate iobuffer %d=>%d", this->capacity, alloc);
                return -ENOMEM;
            }
            this->data     = newData;
            this->capacity = alloc - 1;
            this->data[this->size] = '\0';
        }
        return 0;
    }

    void erase(int from, int to)
    {   
        int n = to - from;
        if( n > 0) {
            memmove( this->data + from, this->data + to, this->size - n);
            this->size -= n;
            this->data[this->size] = '\0';
        }
    }
};

#endif
