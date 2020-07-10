#include "rgba2ayuv.h"
#include "ColorConv.h"
#include <errno.h>

int rgba2ayuv(struct ARGB& rgba, struct AYUV& ayuv)
{
    ayuv.width = rgba.width;
    ayuv.height = rgba.height;
    ayuv.size = ayuv.width * ayuv.height * 4; // AYUV

    ayuv.ayuv_addr = (unsigned char*)malloc(ayuv.size);
    if(ayuv.ayuv_addr == NULL) {
        AYUV_LOG("malloc %d failed: %s\n", ayuv.size, strerror(errno));
        return -1;
    }

    initConvertTable();
    ayuv.preMultiple = 1;

    int isARGB = rgba.isARGB;
    unsigned char* payuv = ayuv.ayuv_addr;
    unsigned char* pargb = rgba.argb_addr;

    for(int i = 0; i < rgba.height; i++) {
        for(int j = 0; j < rgba.width; j++) {
            if(isARGB) {
                unsigned int rgb = ((unsigned int)*(pargb + 0)) << 16 | ((unsigned int)*(pargb + 1)) << 8 | (unsigned int)*(pargb + 2);
                RGB2YUV((payuv + 1), (payuv + 2), (payuv + 3), &rgb);

                *(payuv + 0) = *(pargb + 3);
                unsigned int alpha = *(payuv + 0);
                *(payuv + 1) = (alpha * *(payuv + 1)) >> 8 & 0xFF;
                *(payuv + 2) = (alpha * *(payuv + 2)) >> 8 & 0xFF;
                *(payuv + 3) = (alpha * *(payuv + 3)) >> 8 & 0xFF;
                
                pargb += 4;
                payuv += 4;
            } else {
                *payuv = 255;
                unsigned int rgb = ((unsigned int)*(pargb + 0)) << 16 | ((unsigned int)*(pargb + 1)) << 8 | (unsigned int)*(pargb + 2);
                RGB2YUV((payuv + 1), (payuv + 2), (payuv + 3), &rgb);

                pargb += 3;
                payuv += 4;
            }
        }
    }

    return 0;
}

int overlayNV12(unsigned char* nv12, int w, int h, int xoff, int yoff, struct AYUV& ayuv)
{
    unsigned char* payuv = ayuv.ayuv_addr;
    unsigned char* dst_y_start = nv12 + w * yoff + xoff;
    unsigned char* dst_uv_start = nv12 + w * h + w * yoff / 2 + (xoff & ~0x1);

    for(int i = 0; i < ayuv.height; i++) {
        unsigned char* y = dst_y_start + w * i;
        unsigned char* uv = dst_uv_start + w * i / 2;

        for(int j = 0; j < ayuv.width; j+=2) {
            int alpha = 255 - *(payuv + 0);
            
            //只计算第一个Y点的uv就好, 第二个Y点共用
            *(uv + j) = (((alpha * *(uv + j) / 255) & 0xFF) + *(payuv + 2)) & 0xFF;
            *(uv + j + 1) = (((alpha * *(uv + j + 1) / 255) & 0xFF) + *(payuv + 3)) & 0xFF;

            *y ++ = ((((alpha * *y) / 255) & 0xFF) + *(payuv + 1)) & 0xFF;
            payuv += 4;

            alpha = 255 - *(payuv + 0); 
            *y ++ = ((((alpha * *y) / 255) & 0xFF) + *(payuv + 1)) & 0xFF;
            payuv += 4;
        }
    }
    return 0;
}