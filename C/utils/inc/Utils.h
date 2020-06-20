#ifndef _GOLD_POOL_UTILS_H_
#define _GOLD_POOL_UTILS_H_
#include <stdint.h>

inline void FILL_UINT16_TO_BYTE(uint8_t* byte, uint16_t data, int little_endian = 1)
{
    if(little_endian) {
        *byte++ = data & 0xFF;
        *byte++ = (data >> 8) & 0xFF;
    } else {
        *byte++ = (data >> 8) & 0xFF;
        *byte++ = data & 0xFF;
    }
    return;
}

inline void FILL_UINT32_TO_BYTE(uint8_t* byte, uint32_t data, int little_endian = 1)
{
    if(little_endian) {
        *byte++ = data & 0xFF;
        *byte++ = (data >> 8) & 0xFF;
        *byte++ = (data >> 16) & 0xFF;
        *byte++ = (data >> 24) & 0xFF;
    } else {
        *byte++ = (data >> 24) & 0xFF;
        *byte++ = (data >> 16) & 0xFF;
        *byte++ = (data >> 8) & 0xFF;
        *byte++ = data & 0xFF;
    }
    return;
}

#endif