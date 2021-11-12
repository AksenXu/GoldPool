#ifndef __QRCODE_API_H__
#define __QRCODE_API_H__

#ifdef __cplusplus
extern "C" {
#endif

extern int qrcode_init(void);
extern int qrcode_deinit(void);
extern int qrcode_parser(int width, int height, void *yuvrawframe, char *result,int result_len, int crop_w, int crop_h, int crop_x, int crop_y);

#ifdef __cplusplus
}
#endif

#endif

