#ifndef __ARGB_2_AYUV_H_
#define __ARGB_2_AYUV_H_

#include "color.h"

/**
 * convent rgba to ayuv, notice check the format in rgba, maybe it's only rgb(3bytes), not rgba(4ytes)
 * @param[in] rgba: rgba data
 *            ayuv: ayuv data, ayuv_addr not alloc outside
 * @return 0: convent OK, -1: some error happen
 */
int rgba2ayuv(struct ARGB& rgba, struct AYUV& ayuv);

/**
 * overlay AYUV to MainYUV
 * @param[in]   nv12: nv12 vaddr
 *              w, h: background big YUV resolution, like camera yuv data
 *              xoff, yoff: overlay position of ayuv
 *              ayuv: small overlay yuv, it's often the png decoded logo yuv
 * @return 0: overlay OK, -1: some error happen
 */
int overlayNV12(unsigned char* nv12, int w, int h, int xoff, int yoff, struct AYUV& ayuv);

#endif