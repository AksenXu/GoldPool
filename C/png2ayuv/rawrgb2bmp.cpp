#include <stdio.h>
#include <stdlib.h>
#include "../utils/FileUtils.h"
#include "../utils/inc/Utils.h"

// https://www.cnblogs.com/kingmoon/archive/2011/04/18/2020097.html
// 可以参考ffmpeg的bmpenc.c

int main(int argc, char* argv[])
{
    if(argc != 4) {
        printf("Usage: %s w h file\n", argv[0]);
        printf(" add bmp heade to file\n");
        return -1;
    }

    int w = atoi(argv[1]);
    int h = atoi(argv[2]);
    const char* file = argv[3];
    char* bmpfile = NULL;
    asprintf(&bmpfile, "%s.bmp", file);

    unsigned char* rawData = NULL;
    int len = readFile(file, &rawData);
    if(len < 0) {
        printf("read %s failed\n", file);
        return -1;
    }

    printf("raw bitmap %dx%d %s to %s\n", w, h, file, bmpfile);

    uint32_t bmpDataOffset = 54;
    uint32_t filesize = len + bmpDataOffset;
    
    uint8_t bmp_file_header[14] = {0};
    bmp_file_header[0] = 'B';                                   //bfType
    bmp_file_header[1] = 'M';
    FILL_UINT32_TO_BYTE(&bmp_file_header[2], filesize);         //bfSize
    FILL_UINT16_TO_BYTE(&bmp_file_header[6], 0);    //bfReserved1
    FILL_UINT16_TO_BYTE(&bmp_file_header[8], 0);    //bfReserved2
    FILL_UINT32_TO_BYTE(&bmp_file_header[10], bmpDataOffset);   //bfOffBits

    uint8_t bmp_header[40] = {0};
    FILL_UINT32_TO_BYTE(&bmp_header[0], 40);    //biSize
    FILL_UINT32_TO_BYTE(&bmp_header[4], w);     //biWidth
    FILL_UINT32_TO_BYTE(&bmp_header[8], h);     //biHeight
    FILL_UINT16_TO_BYTE(&bmp_header[12], 1);    //biPlanes
    FILL_UINT16_TO_BYTE(&bmp_header[14], 24);   //biBitCount FIXME: 固定为24bit bmp
    FILL_UINT32_TO_BYTE(&bmp_header[16], 0);    //biCompression
    FILL_UINT32_TO_BYTE(&bmp_header[20], 0);    //biSizeImages
    FILL_UINT32_TO_BYTE(&bmp_header[24], 0);    //biXPelsPerMeter
    FILL_UINT32_TO_BYTE(&bmp_header[28], 0);    //biYPelsPerMeter
    FILL_UINT32_TO_BYTE(&bmp_header[32], 0);    //biClrUsed
    FILL_UINT32_TO_BYTE(&bmp_header[36], 0);    //biClrImportant

    //write bmp header
    writeFile(bmpfile, (const char*)bmp_file_header, 14, 0);
    writeFile(bmpfile, (const char*)bmp_header, 40, 1);

    //write raw bmp data
    writeFile(bmpfile, (const char*)rawData, len, 1);
    free(rawData);

    free(bmpfile);
    return 0;
}