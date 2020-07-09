#include <stdio.h>
#include "png2rgb.h"
#include "argb2ayuv.h"

#include "../utils/FileUtils.h"

int main(int argc, char* argv[])
{
    char* rgbafile = NULL;
    char* rgbfile = NULL;
    char* ayuvfile = NULL;

    unsigned char* pngData = NULL;
    struct ARGB rgba;    // 注意，虽然类的名字是argb，但是从libpng的库里面解码出来的PNG_COLOR_TYPE_RGB_ALPHA是rgba的格式
    struct AYUV ayuv;
    int ret = -1;

    int dst_w = 128;
    int dst_h = 128;
    int dst_size = dst_w * dst_h * 3 / 2;
    unsigned char* nv12 = NULL;
    char* nv12file = NULL;

    if(argc != 2) {
        AYUV_LOG("Usage: %s file.png\n", argv[0]);
        AYUV_LOG(" 1 decode .png file to .rgba raw data\n");
        AYUV_LOG(" 2 convert .rgba raw data to .ayuv raw data\n");
        AYUV_LOG(" 3 overlay .ayuv raw data to .yuv YV12 raw data\n");

        return -1;
    }

    const char* filename = argv[1];
    AYUV_LOG("[%s]: convert %s to %s.ayuv.rgba\n", argv[0], filename, filename);

    int pngLen = readFile(filename, &pngData);
    if(pngLen < -1) {
        AYUV_LOG("[%s]: read png file %s failed\n", argv[0], filename);
        goto exit;
    }

    AYUV_LOG("[%s]: png file %s size %d pngData %p\n", argv[0], filename, pngLen, pngData);

    // 1. decode .png to .rgba
    ret = png_decode_to_argb(pngData, pngLen, rgba);
    if(ret < 0) {
        AYUV_LOG("png_decode_to_argb failed ret = %d", ret);
        goto exit;
    }

    asprintf(&rgbfile, "%s.ayuv.rgb", filename);
    AYUV_LOG("[%s]: write rgb to file %s\n", argv[0], rgbfile);

    if(rgbfile) {
        if(rgba.isARGB == 0) {
            writeFile(rgbfile, (const char*)rgba.argb_addr, rgba.size);
        } else {
            asprintf(&rgbafile, "%s.ayuv.rgba", filename);
            AYUV_LOG("[%s]: write rgba to file %s\n", argv[0], rgbafile);
            writeFile(rgbafile, (const char*)rgba.argb_addr, rgba.size);
            
            //RGBA data, only write RGB, ignore the alpha bytes;
            FILE* fp = fopen(rgbfile, "wb");
            for(int i = 0; i < rgba.size / 4; i++) {
                fwrite(rgba.argb_addr + 4 * i, 1, 3, fp);
            }
            fclose(fp);
        }
    }

    // 2. convert .rgba to .ayuv data
    AYUV_LOG("[%s]: convert rgba to ayuv\n", argv[0]);
    ret = rgba2ayuv(rgba, ayuv);
    if(ret == -1) {
        AYUV_LOG("argb2ayuv failed ret = %d", ret);
        goto exit;
    }

    asprintf(&ayuvfile, "%s.ayuv", filename);
    AYUV_LOG("[%s]: write ayuv data to file %s\n", argv[0], ayuvfile);
    writeFile(ayuvfile, (const char*)ayuv.ayuv_addr, ayuv.size);

    // 3. overlay .ayuv to YV12 data
    nv12 = (unsigned char*) malloc(dst_size);
    memset(nv12, 0, dst_size);
    AYUV_LOG("[%s]: overlay ayuv to nv12 %dx%d\n", argv[0], dst_w, dst_h);
    overlayYUV(nv12, dst_w, dst_h, dst_w / 2, dst_h / 2, ayuv);

    asprintf(&nv12file, "%s.ayuv.nv12", filename);
    AYUV_LOG("[%s]: store nv12 %s\n", argv[0], nv12file);
    if(nv12file) {
        writeFile(nv12file, (const char*)nv12, dst_size);
    }    

exit:
    free(rgbafile);
    free(rgbfile);
    free(ayuvfile);
    free(pngData);
    free(nv12);
    free(nv12file);
    return 0;
}
