#ifndef __COLOR_H__HH__
#define __COLOR_H__HH__

#include <stdlib.h>
#include <string.h>

struct ARGB {
    ARGB() {
        memset(this, 0, sizeof(struct ARGB));
    }
    
    unsigned char* argb_addr;
    //png resolution
    int width;
    int height;
    
    int isARGB;
    int size;
};

/*
 * 1 store the alpha and YUV data to memory and return to user
 * 2 if A exist, multiple the alpha when convert going
 * 
 * AYUV Layout:
 * |AYUVAYUVAYUV-----------------------|
 * |-----------------------------------|
 * |-----------------------------------|
 * |-----------------------------------|
 * |-----------------------------------|
 * |-----------------------AYUVAYUVAYUV|
 */
 
/*
 * decode png to struct AYUV
 */
struct AYUV
{
    AYUV() {
        memset(this, 0, sizeof(struct AYUV));
    }
    ~AYUV() {
        free(ayuv_addr);
    }

    unsigned char* ayuv_addr;

    int width;
    int height;
    
    int size;
    //if alpha exist, pre-multiple the yuv data with alpha
    int preMultiple;
};

#define AYUV_LOG  printf

#endif