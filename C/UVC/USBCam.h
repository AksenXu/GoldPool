#ifndef _CARMEDIA_USB_CAM_H_
#define _CARMEDIA_USB_CAM_H_

#include <linux/videodev2.h>
#include "CameraDefine.h"

struct v4l2Buffer {
    void *      start;
    size_t      length;
};

#define CAMERA_BUFFERS 3

enum USBCameraType {
    USBCAMERA_NONE = -1,
    USBCAMERA_YUV = 0,
    USBCAMERA_JPEG = 1,
    USBCAMERA_H264 = 2
};
namespace carmedia {

class USBCam
{
public:
    //check USBCamera with cameraid type
    static USBCameraType getUSBCameraType(int cameraid = USB_CAMERA_BACK);
    
	USBCam(int cameraid);
    virtual ~USBCam();

    void queryCamInfo(uint32_t* width, uint32_t* height, uint32_t* format);
    void setCamFormat(uint32_t width, uint32_t height, uint32_t fps, uint32_t format);

    int startStreaming();
    int stopStreaming();

protected:

    int                     initDevice();
    int                     deinitDevice();
    int                     getBuffer(v4l2_buffer* buf);
    void                    putBuffer(v4l2_buffer* buf);
    uint32_t                miFrameNumber;
    struct v4l2Buffer       mVideoBuffer[CAMERA_BUFFERS];

private:
    const char*             getCameraType(int format);
    static int              checkCamCapbility(int fd);
    void                    closefd();
    
    uint32_t                miWidth;
    uint32_t                miHeight;
    uint32_t                miFps;
    uint32_t                miFormat;
    int						mVideoFd;
    int                     mStreamingOn;
    int                     miCameraID;
    int                     EnumerateFmt(void);
    int                     EnumerateFrameSize(int fmt);
    int                     EnumerateFrameIntervals(int fmt, int width, int height);
    
    int                     mForcePowerDown;
};
};
#endif