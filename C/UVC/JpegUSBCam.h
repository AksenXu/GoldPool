#ifndef _JPEG_USB_CAM_H_
#define _JPEG_USB_CAM_H_
/*
 *  For Jpeg USB/UVC Camera
 */
#include "USBCam.h"
namespace carmedia {

class JpegUSBCam: public USBCam
{
public:
	JpegUSBCam(int cameraId, int width, int height, int fps);
    virtual ~JpegUSBCam();
	
    int getJPEGFrame(uint8_t* &jpeg, int &len);

private:
    int checkThermalDropFrame();
    
    int64_t mFrameTime;
    int mFrameFetchInterval;
    int mJPEGFps;
};
};
#endif

