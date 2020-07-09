#ifndef __PNG_2_AYUV_H_
#define __PNG_2_AYUV_H_
#include <stdlib.h>
#include <string.h>

//header length of bytes to check if it is a valid png file
#define PNGSIGSIZE  8

#define CC_BREAK_IF(x) if(x) break;

/*
 * 1 use libpng to decode png memory
 * 2 store the alpha and YUV data to memory and return to user
 * 
 * AYUV Layout:
 * |-----------------------------------|
 * |                A                  |
 * |-----------------------------------|
 * |-----------------------------------|
 * |                Y                  |
 * |-----------------------------------|
 * |-----------------------------------|
 * |                UV                 |
 * |-----------------------------------|
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
    unsigned char* yaddr;

    //png resolution
    int width;
    int height;
    
    //almost equal to width except some align needed
    int stride;
    
    //YV12/NV12/NV21...
    //TODO: now only support NV21
    int format;
    int size;
    int yuvsize;
    
    //if alpha exist, pre-multiple the yuv data with alpha
    int preMultiple;
};

struct ARGB {
    ARGB() {
        memset(this, 0, sizeof(struct ARGB));
    }
    
    unsigned char* argb_addr;
    //png resolution
    int width;
    int height;
    
    int png_format;
    int size;
};

/*
 * overlay struct AYUV to MainYUV
 */
struct MainYUV
{
    unsigned char* vaddr;
    
    //yuv resolution
    int width;
    int height;
    int stride;
    
    //AYUV position
    int xoff;
    int yoff;
    
    //TODO: now only support NV21
    int format;
};

/**
 * Decode png picture memory to struct ARGB
 * @param[in] pngData: pointer to png memory
 *            pngLen: png memory length
 * @param[out] argb: struct ARGB to store the ARGB info
 * @return 0: decode OK, -1: decode failed
 */
int png_decode_to_argb(unsigned char* pngData, int pngLen, struct ARGB& argb);

/**
 * overlay AYUV to MainYUV
 * @param[in] mainyuv: big main YUV, it's often video decoded yuv
 *            ayuv: small overlay yuv, it's often the png decoded logo yuv
 * @return 0: overlay OK, -1: some error happen
 */
int overlayYUV(struct MainYUV& mainyuv, struct AYUV& ayuv);

#define AYUV_LOG  printf

#endif