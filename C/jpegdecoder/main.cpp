#include <stdio.h>
#include "JpegDecoder.h"
#include "carutils/FileUtil.h"
#include "carutils/IOBuffer.h"

using namespace carutils;

int main(int argc, char* argv[])
{
    const char* jpegFile = "/data/test.jpg";
    const char* yuvFile = "/data/test.yuv";
    
    int w = 430, h = 430;
    int len = 430*430*3/2;
    uint8_t* out = (uint8_t*) malloc(len);

    IOBuffer jpegBuf;

    loadFile(jpegFile, &jpegBuf);

    jpegDecode((uint8_t*)jpegBuf.data, jpegBuf.size, jpegBuf.size, out, JPEG_YUV420_PLANAR);
    
    writeFile(yuvFile, (const char*)out, len);

    free(out);
    return 0;
}
