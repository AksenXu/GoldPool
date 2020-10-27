#ifndef CARMEDIA_JPEG_DECODER_H_
#define CARMEDIA_JPEG_DECODER_H_

enum {
    JPEG_YUV420_PLANAR,
    JPEG_YCbCr420_SEMIPLANAR,
    JPEG_YCrCb420_SEMIPLANAR
};

extern void jpegDecode(uint8_t* data, uint32_t length, uint32_t dataSize, uint8_t* out, int out_format);

#endif
