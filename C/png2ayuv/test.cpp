#include <stdio.h>
#include "png2ayuv.h"

#include "../utils/FileUtils.h"

int main(int argc, char* argv[])
{
    char* ayuvfile = NULL;
    char* yuvfile = NULL;
    unsigned char* pngData = NULL;
    struct AYUV ayuv;
    int ret = -1;
    
    if(argc != 2) {
        AYUV_LOG("Usage: %s file.png\n", argv[0]);
        return -1;
    }

    const char* filename = argv[1];
    AYUV_LOG("%s convert %s to %s.yuv and %s.ayuv\n", argv[0], filename, filename, filename);

    int pngLen = readFile(filename, &pngData);
    if(pngLen < -1) {
        AYUV_LOG("read png file %s failed\n", filename);
        goto exit;    
    }

    AYUV_LOG("png file %s size is %d pngData %p\n", filename, pngLen, pngData);

    ret = png_decode_to_ayuv(pngData, pngLen, ayuv);
    if(ret < 0) {
        AYUV_LOG("png_decode_to_ayuv failed ret = %d", ret);
        goto exit;
    }

    asprintf(&yuvfile, "%s.yuv", filename);
    if(yuvfile) {
        writeFile(yuvfile, (const char*)ayuv.yaddr, ayuv.yuvsize);
    }

    asprintf(&ayuvfile, "%s.ayuv", filename);
    if(ayuvfile) {
        writeFile(ayuvfile, (const char*)ayuv.vaddr, ayuv.size);
    }

exit:
    free(yuvfile);
    free(ayuvfile);
    free(pngData);
    return 0;
}
