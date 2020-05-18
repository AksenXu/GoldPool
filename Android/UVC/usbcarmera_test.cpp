#include <stdio.h>
#include <errno.h>
#include "JpegUSBCam.h"
#include "carutils/String8.h"
#include "carutils/FileUtil.h"

#include "carutils/WLogger.h"
static WLogger * myLog = wlogGet("@main");

using namespace carmedia;
using namespace carutils;

int main(int argc, char* argv[])
{
    wlogSetSetting("prefix=carmedia_");
    JpegUSBCam* usbcam = new JpegUSBCam(/*CAMERA1_ID*/2, 640, 480, 15);

    int maxLen = 640*480*3/2;
    unsigned char* jpegData = (unsigned char*)malloc(maxLen);

    for(int i = 0; i < 100; i ++) {
        int jpegSize = usbcam->getJPEGFrame(jpegData, maxLen);

        WLOGI(myLog, "got jpegSize %d\n", jpegSize);

        if(i % 10 == 0 && jpegSize > 0) {
            String8 filename = String8::format("/storage/sdcard0/jpeg_%d.jpg", i);
            int write = writeFile(filename.string(), (const char*)jpegData, jpegSize);
            if(write) {
                WLOGI(myLog, "writeFile %s size %d OK\n", filename.string(), jpegSize);
            } else {
                WLOGI(myLog, "writeFile %s size %d failed %s\n", filename.string(), jpegSize, strerror(errno));
            }
        }
    }

    free(jpegData);
    delete usbcam;
    return 0;
}