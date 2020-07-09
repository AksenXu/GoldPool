#include <stdio.h>
#include "png2rgb.h"

#include "../utils/FileUtils.h"

int main(int argc, char* argv[])
{
    char* argbfile = NULL;
    unsigned char* pngData = NULL;
    struct ARGB argb;
    int ret = -1;
    
    if(argc != 2) {
        AYUV_LOG("Usage: %s file.png\n", argv[0]);
        AYUV_LOG(" 1 decode .png file to .argb raw data\n");
        AYUV_LOG(" 2 convert .argb raw data to .bmp file\n");
        AYUV_LOG(" 3 convert .argb raw data to .ayuv raw data\n");
        return -1;
    }

    const char* filename = argv[1];
    AYUV_LOG("[%s]: convert %s to %s.argb and %s.ayuv\n", argv[0], filename, filename, filename);

    int pngLen = readFile(filename, &pngData);
    if(pngLen < -1) {
        AYUV_LOG("[%s]: read png file %s failed\n", argv[0], filename);
        goto exit;    
    }

    AYUV_LOG("[%s]: png file %s size %d pngData %p\n", argv[0], filename, pngLen, pngData);

    ret = png_decode_to_argb(pngData, pngLen, argb);
    if(ret < 0) {
        AYUV_LOG("png_decode_to_argb failed ret = %d", ret);
        goto exit;
    }

    asprintf(&argbfile, "%s.argb", filename);
    if(argbfile) {
        writeFile(argbfile, (const char*)argb.argb_addr, argb.size);
    }

exit:
    free(argbfile);
    free(pngData);
    return 0;
}
