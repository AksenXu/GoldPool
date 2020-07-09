#include <stdlib.h>
#include <stdio.h>
#include "ColorConv.h"

int32_t sConvertTable[13][256];  
static int tableInited = 0;

void initConvertTable()
{
    if(tableInited) {
        return;
    }
    
    int32_t coef[13][2] = 
    {
        //yuv -> rgb
        { -128, 292}, { -128, 101}, { -128, 149}, { -128, 520},
        //rgb -> yuv
        {0, 77}, {0, 150}, {0, 29},
        {0, -38}, {0, -74}, {0, 112},
        {0, 157}, {0, -132}, {0, -26}
    };
    
    for(int i = 0; i < 13; i ++) {
        for(int j = 0; j < 256; j++) {
            sConvertTable[i][j] = (j + coef[i][0]) * coef[i][1];
        }
    }
    
    tableInited = 1;
}
/**
 * Clip a signed integer value into the 0-255 range.
 * @param a value to clip
 * @return clipped value
 */
static inline uint8_t av_clip_uint8_c(int a)
{
    if (a&(~0xFF)) return (-a)>>31;
    else           return a;
}

/*
R' = Y' + 1.140*V' = (256Y + 292V) >> 8
G' = Y' - 0.394*U' - 0.581*V' = (256Y - 101U - 149V) >> 8
B' = Y' + 2.032*U' = (256Y + 520U) >> 8
*/
void YUV2RGB(uint8_t* y, uint8_t* u, uint8_t* v, uint32_t* rgb)
{
    int yy, ug, ub, vg, vr, r, g, b;
    //r = av_clip_uint8_c((((int)*y << 8) + (((int)*v - 128)  * 292)) >> 8);
    //g = av_clip_uint8_c((((int)*y << 8) - (((int)*u - 128)  * 101) - (((int)*v - 128)  * 149)) >> 8);
    //b = av_clip_uint8_c((((int)*y << 8) + (((int)*u - 128)  * 520)) >> 8);

    r = av_clip_uint8_c((((int)*y << 8) + sConvertTable[0][*v]) >> 8);
    g = av_clip_uint8_c((((int)*y << 8) - sConvertTable[1][*u] - sConvertTable[2][*v]) >> 8);
    b = av_clip_uint8_c((((int)*y << 8) + sConvertTable[3][*u]) >> 8);

    *rgb = ( ((r<<16) & 0xFF0000) | ((g<<8) & 0xFF00) | (b & 0xFF));
    return;
}

/*
 Y =  0.299R + 0.587G + 0.114B = (77R + 150G + 29B ) >>8
 U = -0.147R - 0.289G + 0.436B = (-38R - 74G + 112B) >> 8
 V =  0.615R - 0.515G - 0.100B = (157R - 132G - 26B) >> 8
*/
void RGB2YUV(uint8_t* y, uint8_t* u, uint8_t* v, uint32_t* rgb)
{
    int r, g, b;
    r = ((*rgb & 0xFF0000) >> 16);
    g = ((*rgb & 0xFF00) >> 8);
    b = (*rgb & 0xFF);

    //*y = av_clip_uint8_c((77 * r + 150 * g + 29 * b) >> 8);
    //*u = av_clip_uint8_c(((-38 * r - 74 * g + 112 * b) >> 8) + 128);
    //*v = av_clip_uint8_c(((157 * r - 132 * g - 26 * b) >> 8) + 128);

    *y = av_clip_uint8_c((sConvertTable[4][r] + sConvertTable[5][g] + sConvertTable[6][b]) >> 8);
    *u = av_clip_uint8_c(((sConvertTable[7][r] + sConvertTable[8][g] + sConvertTable[9][b]) >> 8) + 128);
    *v = av_clip_uint8_c(((sConvertTable[10][r] + sConvertTable[11][g] + sConvertTable[12][b]) >> 8) + 128);
    
    return;
}

void YUVtoRGB(uint8_t* y, uint8_t* u, uint8_t* v, int y_stride, int height, 
            uint32_t w, uint32_t h, uint32_t* rgb, bool yuv2rgb, int mirror)
{
    uint32_t uv_stride = y_stride / 2;
    
    for(uint32_t i = 0; i < h; i++) {
        for(uint32_t j = 0; j < w; j++) {
            int offset = j;
            if(mirror) {
                offset = w - j;
            }
            if(yuv2rgb) {
                YUV2RGB((y + offset), (u + offset / 2), (v + offset / 2), rgb);
            } else {
                RGB2YUV((y + offset), (u + offset / 2), (v + offset / 2), rgb);
            }
            rgb ++;
        }
        y += y_stride;
        u += (i % 2) * uv_stride;
        v += (i % 2) * uv_stride;
    }
}

void YUVsptoRGB(uint8_t* y, uint8_t* uv, int y_stride, int height, 
            uint32_t w, uint32_t h, uint32_t* rgb, bool yuv2rgb, int mirror)
{
    uint32_t uv_stride = y_stride;
    
    for(uint32_t i = 0; i < h; i++) {
        for(uint32_t j = 0; j < w; j++) {
            int offset = j;
            if(mirror) {
                offset = w - j;
            }
            if(yuv2rgb) {
                YUV2RGB((y + offset), (uv + (offset & ~1)), (uv + (offset & ~1) + 1), rgb);
            } else {
                RGB2YUV((y + offset), (uv + (offset & ~1)), (uv + (offset & ~1) + 1), rgb);
            }
            rgb ++;
        }
        y += y_stride;
        uv += (i % 2) * uv_stride;
    }
}

