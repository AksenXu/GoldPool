#ifndef __COLOR_CONVERT_H_
#define __COLOR_CONVERT_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

//prebuilt yuv<-->rgb table to optimize process speed
//13 multiple in yuv<-->rgb convert
extern int32_t sConvertTable[13][256];  

extern void initConvertTable();
extern void YUVtoRGB(uint8_t* y, uint8_t* u, uint8_t* v, int y_stride, int height, 
            uint32_t w, uint32_t h, uint32_t* rgb, bool yuv2rgb, int mirror);
extern void YUVsptoRGB(uint8_t* y, uint8_t* uv, int y_stride, int height, 
            uint32_t w, uint32_t h, uint32_t* rgb, bool yuv2rgb, int mirror);

extern void RGB2YUV(uint8_t* y, uint8_t* u, uint8_t* v, uint32_t* rgb);
extern void YUV2RGB(uint8_t* y, uint8_t* u, uint8_t* v, uint32_t* rgb);

#endif