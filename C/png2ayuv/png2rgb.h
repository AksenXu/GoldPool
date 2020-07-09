#ifndef __PNG_2_ARGB_H_
#define __PNG_2_ARGB_H_

#include "color.h"

//header length of bytes to check if it is a valid png file
#define PNGSIGSIZE  8

#define CC_BREAK_IF(x) if(x) break;

/**
 * Decode png picture memory to struct ARGB
 * @param[in] pngData: pointer to png memory
 *            pngLen: png memory length
 * @param[out] argb: struct ARGB to store the ARGB info
 * @return 0: decode OK, -1: decode failed
 */
int png_decode_to_argb(unsigned char* pngData, int pngLen, struct ARGB& argb);

#endif