#include <unistd.h>
#include "JpegUSBCam.h"
#include "carutils/PropUtil.h"
#include "carutils/Timers.h"

#include "carutils/WLogger.h"
static WLogger* myLog = wlogGet("@JpegUSBCam");

namespace carmedia {

JpegUSBCam::JpegUSBCam(int cameraId, int width, int height, int fps) 
        : USBCam(cameraId) 
{
    mFrameTime = 0ll;    
    mFrameFetchInterval = 0;
    mJPEGFps = fps;
    WLOGI(myLog, "JpegUSBCam %p create", this);

    setCamFormat(width, height, mJPEGFps, V4L2_PIX_FMT_MJPEG);
    initDevice();
    startStreaming();
}

JpegUSBCam::~JpegUSBCam() 
{
    stopStreaming();
    deinitDevice();
    WLOGI(myLog, "JpegUSBCam %p destroy", this);
}

int JpegUSBCam::getJPEGFrame(uint8_t* &jpeg, int& len)
{
    WLOGV(myLog, "getJPEGFrame +");
    int jpegLen = -1;
    
    struct v4l2_buffer buf;    
    if(getBuffer(&buf) == 0) {

        //drop frames if thermal high
        if(checkThermalDropFrame()) {
            putBuffer(&buf);
            return -1;
        }

        jpegLen = buf.bytesused;
        if(jpegLen > len) {
            jpeg = (uint8_t*) realloc(jpeg, jpegLen);
            WLOGI(myLog, "getJPEGFrame Jpeg data size %d => %d", len, jpegLen);
            len = jpegLen;
        }

        memcpy(jpeg, mVideoBuffer[buf.index].start, jpegLen);
        putBuffer(&buf);
    } else {
        usleep(100000);
        return -1;
    }

    //no matter the camera support the request resolution and fps or not
    //if the interval is too small between both frames, just sleep a while
    int64_t want_interval = (int64_t)1000 / mJPEGFps;
    int64_t now = carutils::uptimeMillis();
    int64_t got_interval = now - mFrameTime;
    if((mFrameTime != 0ll) && (got_interval > 0) && (got_interval < want_interval)) {
        usleep((want_interval - got_interval) * 1000ll);
        //ALOGD("want_interval %lld got_interval %lld; Want sleep %lld got sleep %lld", 
                //want_interval , got_interval,
                //want_interval - got_interval, systemTime(CLOCK_MONOTONIC) / 1000000ll - now);
    }

    mFrameTime = carutils::uptimeMillis();

    WLOGV(myLog, "getJPEGFrame -");
    return jpegLen;
}

int JpegUSBCam::checkThermalDropFrame()
{
    if((miFrameNumber % 30) == 0) {
        mFrameFetchInterval = carutils::getPropertyInt("sys.thermalhigh.camera.interval", 0);
    }

    if(mFrameFetchInterval <= 1) return 0;

    return (miFrameNumber % mFrameFetchInterval);
}
};