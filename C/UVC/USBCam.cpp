#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <cutils/properties.h>
#include "USBCam.h"
#include "carutils/Utils.h"
#include "carutils/PropUtil.h"

#include "carutils/WLogger.h"
static WLogger * myLog = wlogGet("@UsbCam");

using namespace carutils;

#define MAX_CAMERA_NODE             10

namespace carmedia {

USBCameraType USBCam::getUSBCameraType(int cameraid)
{
    enum USBCameraType type = USBCAMERA_NONE;
    int fd = -1;
    int max_node_idx = MAX_CAMERA_NODE;
    const char* property = USB_CAMERA_PROPERTY_BACK;

    //if left and right usb camera exist, only try special node, not try all the node.
    //if only one usb camera exist, cameraId=2, try all the possible node
    if(cameraid == USB_CAMERA_LETT) { //left usb camera
        max_node_idx = 0;
        property = USB_CAMERA_PROPERTY_LEFT;
    } else if(cameraid == USB_CAMERA_RIGHT) { //right usb camera
        max_node_idx = 0;
        property = USB_CAMERA_PROPERTY_RIGHT;
    }
    
    //try MAX_CAMERA_NODE node until we can success
    for(int i = -1; i < max_node_idx; i++) {
        char node[PROPERTY_VALUE_MAX];
        memset(node, 0, sizeof(node));
        if(i == -1) {
            property_get(property, node, "/dev/video0");
            WLOGD(myLog, "sys.usb.camnode property : %s", node);
        } else {
            sprintf(node, "/dev/video%d", i);
        }
        
        fd = open(node, O_RDWR);
        if(fd < 0) {
            WLOGE(myLog, "USB Camera %s open failed: %s", node, strerror(errno));
            continue;
        } else {
            WLOGD(myLog, "USB Camera %s opened: fd(%d)", node, fd);
            checkCamCapbility(fd);

            struct v4l2_fmtdesc fmt;
            CLEAR(fmt);
            fmt.index = 0;
            fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

            while (ioctl(fd, VIDIOC_ENUM_FMT, &fmt) == 0) {
                if(fmt.pixelformat == V4L2_PIX_FMT_H264) {
                    WLOGD(myLog, "Found H264 UVC Video Format");
                    type = USBCAMERA_H264;
                    break;
                } else if(fmt.pixelformat == V4L2_PIX_FMT_MJPEG) {
                    WLOGD(myLog, "Found JPEG UVC Video Format");
                    type = USBCAMERA_JPEG;
                } else if(fmt.pixelformat == V4L2_PIX_FMT_YUYV) {
                    WLOGD(myLog, "Found YUV UVC Video Format");
                    type = USBCAMERA_YUV;
                }
                fmt.index++;
            }
            
            CLOSE_FD(fd);
            
            if(type == USBCAMERA_NONE) {
                break;
            }
        }
    }

    return type;
}

USBCam::USBCam(int cameraid) 
{
    CLEAR(mVideoBuffer);
    miWidth = miHeight = 0;
    miFps = 0;

    miFormat = 0;
    miFrameNumber = 0;
    mVideoFd = -1;
    mStreamingOn = 0;
    miCameraID = cameraid;
    WLOGI(myLog, "USBCam %p create", this);
}

USBCam::~USBCam() 
{
    WLOGI(myLog, "USBCam %p destroy", this);
}

void USBCam::setCamFormat(uint32_t width, uint32_t height, uint32_t fps, uint32_t format) 
{
    miWidth = width;
    miHeight = height;
    miFps = fps;
    miFormat = format;
    WLOGI(myLog, "setCamFormat width %d height %d fps %d format %d", width, height, fps, format);
    return;
}

void USBCam::queryCamInfo(uint32_t* width, uint32_t* height, uint32_t* format) 
{
    *width = miWidth;
    *height = miHeight;
    *format = miFormat;
    return;
}

int USBCam::checkCamCapbility(int fd)
{
    int ret = -1;
    struct v4l2_capability cap;
    struct v4l2_cropcap cropcap;
    struct v4l2_crop crop;
    
    if (ioctl (fd, VIDIOC_QUERYCAP, &cap)) {
        WLOGE(myLog, "VIDIOC_QUERYCAP fail\n");
        goto end;
    }

    if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
        WLOGE(myLog, "VIDIOC_QUERYCAP fail\n");
        goto end;
    }

    if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
        WLOGE(myLog, "device does not support streaming i/o\n");
        goto end;
    }
    /* Print capability infomations */
    WLOGD(myLog, "Capability Informations:\n");
    WLOGD(myLog, "driver:%s, card:%s, bus_info:%s, version:%08X\n", 
            cap.driver, cap.card, cap.bus_info, cap.version);

    /* Select video input, video standard and tune here. */
    CLEAR(cropcap);
    cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (0 == ioctl (fd, VIDIOC_CROPCAP, &cropcap)) {
        crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        crop.c = cropcap.defrect; /* reset to default */

        if (-1 == ioctl (fd, VIDIOC_S_CROP, &crop)) {
            WLOGE(myLog, "device does not support set crop\n");
        }
    }

    ret = 0;
end:    
    return ret;
}

int USBCam::EnumerateFmt(void)
{
    struct v4l2_fmtdesc fmt;
    CLEAR(fmt);
    fmt.index = 0;
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    while (ioctl(mVideoFd, VIDIOC_ENUM_FMT, &fmt) == 0) {
        EnumerateFrameSize(fmt.pixelformat);
        fmt.index++;
    }

    return 0;
}

int USBCam::EnumerateFrameSize(int fmt)
{
    // frame size
    struct v4l2_frmsizeenum frmsize;
    CLEAR(frmsize);
    frmsize.index = 0;
    frmsize.pixel_format = fmt;

    while (ioctl(mVideoFd, VIDIOC_ENUM_FRAMESIZES, &frmsize) == 0) {
        switch (frmsize.type) {
        case V4L2_FRMSIZE_TYPE_DISCRETE:
            if (EnumerateFrameIntervals(fmt, frmsize.discrete.width, frmsize.discrete.height))
                WLOGD(myLog, "%c%c%c%c, %dx%d", pixfmtstr(fmt), frmsize.discrete.width, frmsize.discrete.height);
            break;
        case V4L2_FRMSIZE_TYPE_STEPWISE:
            WLOGE(myLog, "device not supports sizes %dx%d to %dx%d using %dx%d increments",
                  frmsize.stepwise.min_width, frmsize.stepwise.min_height,
                  frmsize.stepwise.max_width, frmsize.stepwise.max_height,
                  frmsize.stepwise.step_width, frmsize.stepwise.step_height );
            break;
        case V4L2_FRMSIZE_TYPE_CONTINUOUS:
            WLOGE(myLog, "device not supports all sizes %dx%d to %dx%d",
                  frmsize.stepwise.min_width, frmsize.stepwise.min_height,
                  frmsize.stepwise.max_width, frmsize.stepwise.max_height );
            break;
        default:
            WLOGE(myLog, "device not supports type %d", frmsize.type);
            return -EINVAL;
        }
        frmsize.index++;
    }

    return 0;
}

int USBCam::EnumerateFrameIntervals(int fmt, int width, int height)
{
    int ret;
    struct v4l2_frmivalenum frmivalenum;
    CLEAR(frmivalenum);
    frmivalenum.index = 0;
    frmivalenum.pixel_format = fmt;
    frmivalenum.width = width;
    frmivalenum.height = height;

    while ((ret = ioctl(mVideoFd, VIDIOC_ENUM_FRAMEINTERVALS, &frmivalenum)) == 0) {
        switch (frmivalenum.type) {
        case V4L2_FRMIVAL_TYPE_DISCRETE:    //	Discrete frame interval.
            WLOGD(myLog, "%c%c%c%c, %dx%d, %dfps", pixfmtstr(fmt),
                  width, height, frmivalenum.discrete.denominator);
            break;
        case V4L2_FRMIVAL_TYPE_CONTINUOUS:  //	Continuous frame interval.
        case V4L2_FRMIVAL_TYPE_STEPWISE:    //	Step-wise defined frame interval.
        default:
            WLOGE(myLog, "VIDIOC_ENUM_FRAMEINTERVALS type not supported (type %d)", frmivalenum.type);
            break;
        }
        frmivalenum.index++;
    }

    return ret;
}

int USBCam::startStreaming()
{
    WLOGD(myLog, "startStreaming +");
    if( (mVideoFd >= 0) && !mStreamingOn ) {
        for (uint32_t i = 0; i < CAMERA_BUFFERS; ++i) {
            struct v4l2_buffer buf;
            CLEAR (buf);
            buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            buf.memory      = V4L2_MEMORY_MMAP;
            buf.index       = i;
            
            if (ioctl (mVideoFd, VIDIOC_QUERYBUF, &buf)) {
                WLOGE(myLog,  "startStreaming VIDIOC_QUERYCAP fail\n");
                break;
            }
            
            if (mVideoBuffer[i].start == 0) {
                WLOGW(myLog, "mVideoBuffer[%d].start is NULL", i);
                break;
            }

            if (ioctl(mVideoFd, VIDIOC_QBUF, &buf)) {
                WLOGE(myLog, "VIDIOC_QBUF index:%d Error:%s", i, strerror(errno));
            } else {
                WLOGI(myLog, "startStreaming VIDIOC_QBUF %d OK", i);
            }
        }

    	enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        if (ioctl (mVideoFd, VIDIOC_STREAMON, &type)) {
            WLOGE(myLog, "StartStreaming: Unable to start capture: %s", strerror(errno));
            return -1;
        }
        mStreamingOn = true;
        WLOGI(myLog, "startStreaming VIDIOC_STREAMON OK");
    } else {
        WLOGE(myLog, "startStreaming failed mVideoFd %d mStreamingOn %d", mVideoFd, mStreamingOn);
    }
    WLOGD(myLog, "startStreaming -");
    return 0;
}

int USBCam::stopStreaming()
{
    WLOGD(myLog, "stopStreaming + mVideoFd %d mStreamingOn %d", mVideoFd, mStreamingOn);
    if ((mVideoFd >= 0) && mStreamingOn) {
        enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        if (ioctl (mVideoFd, VIDIOC_STREAMOFF, &type)) {
            WLOGE(myLog, "StopStreaming: Unable to stop capture: %s", strerror(errno));
            return -1;
        }

        mStreamingOn = false;
        WLOGI(myLog, "stopStreaming VIDIOC_STREAMOFF OK");
    }
    WLOGD(myLog, "stopStreaming -");

    deinitDevice();
    return 0;
}

int USBCam::deinitDevice()
{
    WLOGD(myLog, "deinitDevice + close mVideoFd %d", mVideoFd);
    for (int i = 0; i < CAMERA_BUFFERS; i++) {
        if(mVideoBuffer[i].start == 0)
            break;

        WLOGD(myLog, "UVC munmap mVideoBuffer[%d].start %p" , i, mVideoBuffer[i].start);
        if (munmap(mVideoBuffer[i].start, mVideoBuffer[i].length) < 0) {
			WLOGE(myLog, "StopStreaming: Unmap failed %s", strerror(errno));
		}
        mVideoBuffer[i].start = 0;
	}    
    
    closefd();

    WLOGD(myLog, "deinitDevice -");
    return 0;
}

int USBCam::initDevice()
{
    struct v4l2_format fmt;
    struct v4l2_streamparm setfps;
    struct v4l2_requestbuffers req;
    unsigned int min;
    WLOGI(myLog, "initDevice USBCam width %d, height %d fps %d format %s", 
                    miWidth, miHeight, miFps, getCameraType(miFormat));

    int max_node_idx = MAX_CAMERA_NODE;
    const char* property = USB_CAMERA_PROPERTY_BACK;

    //if left and right usb camera exist, only try special node, not try all the node.
    //if only one usb camera exist, cameraId=2, try all the possible node
    if(miCameraID == USB_CAMERA_LETT) { //left usb camera
        max_node_idx = 0;
        property = USB_CAMERA_PROPERTY_LEFT;
    } else if(miCameraID == USB_CAMERA_RIGHT) { //right usb camera
        max_node_idx = 0;
        property = USB_CAMERA_PROPERTY_RIGHT;
    }

    //try MAX_CAMERA_NODE node until we can success
    for(int i = -1; i < max_node_idx; i++) {
        char node[PROPERTY_VALUE_MAX];
        memset(node, 0, sizeof(node));
        if(i == -1) {
            property_get(property, node, "/dev/video0");
            WLOGD(myLog, "sys.usb.camnode property : %s", node);
        } else {
            sprintf(node, "/dev/video%d", i);
        }
        
        mVideoFd = open(node, O_RDWR);
        if(mVideoFd < 0) {
            WLOGE(myLog, "USB Camera %s open failed: %s", node, strerror(errno));
            continue;
        } else {
            WLOGI(myLog, "USB Camera %s opened: miCameraID(%d) mVideoFd(%d)", node, miCameraID, mVideoFd);

            if (checkCamCapbility(mVideoFd)) {
                WLOGE(myLog, "checkCamCapbility fail\n");
                goto err;
            }

            /* enum video formats. */
            EnumerateFmt();

            /* set video formats. */
            CLEAR(fmt);
            // enum v4l2_buf_type { 
            // V4L2_BUF_TYPE_VIDEO_CAPTURE        = 1, 
            // V4L2_BUF_TYPE_VIDEO_OUTPUT         = 2, 
            // V4L2_BUF_TYPE_VIDEO_OVERLAY        = 3, 
            fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            if (ioctl (mVideoFd, VIDIOC_G_FMT, &fmt) < 0) {
                WLOGE(myLog, "device does not support get fmt\n");
            }

            fmt.fmt.pix.width       = miWidth;
            fmt.fmt.pix.height      = miHeight;
            fmt.fmt.pix.pixelformat = miFormat;
            fmt.fmt.pix.field       = V4L2_FIELD_ANY;//V4L2_FIELD_INTERLACED;
            fmt.fmt.pix.colorspace  = V4L2_COLORSPACE_JPEG;
            if (ioctl (mVideoFd, VIDIOC_S_FMT, &fmt)) {
                WLOGE(myLog, "set format fail\n");
                goto err;
            }

            if (ioctl (mVideoFd, VIDIOC_G_FMT, &fmt) < 0) {
                WLOGE(myLog, "get format fail\n");
                goto err;
            }

            //field: https://blog.csdn.net/smilefyx/article/details/19005113
            WLOGD(myLog, "USB Camera Use Format:\n");
            WLOGD(myLog, "type:%d, width:%d, height:%d, format:%s, field:%d, fps:%d\n", 
                fmt.type, fmt.fmt.pix.width, fmt.fmt.pix.height, 
                getCameraType(fmt.fmt.pix.pixelformat), fmt.fmt.pix.field, miFps);

            if(fmt.fmt.pix.pixelformat != miFormat) {
                WLOGE(myLog, "USBCamera Format Failed request:%s get: %s",
                    getCameraType(miFormat),
                    getCameraType(fmt.fmt.pix.pixelformat));
                goto err;
            }            
            
            min = fmt.fmt.pix.width * 2;
            if (fmt.fmt.pix.bytesperline < min)
                fmt.fmt.pix.bytesperline = min;

            min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
            if (fmt.fmt.pix.sizeimage < min)
                fmt.fmt.pix.sizeimage = min;

            CLEAR(setfps);
            setfps.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            setfps.parm.capture.timeperframe.numerator = 1;
            setfps.parm.capture.timeperframe.denominator = miFps;
            if (ioctl(mVideoFd, VIDIOC_S_PARM, &setfps)) {
                WLOGE(myLog, "set fps fail\n");
                goto err;
            }

            CLEAR(req);
            req.count   = CAMERA_BUFFERS;
            req.type    = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            req.memory  = V4L2_MEMORY_MMAP;
            if (ioctl (mVideoFd, VIDIOC_REQBUFS, &req)) {
                WLOGE(myLog,  "VIDIOC_QUERYCAP fail\n");
                goto err;
            }

            if (req.count < 2) {
                WLOGE(myLog,  "Insufficient buffer memory\n");
                goto err;
            }

            for (uint32_t n = 0; n < req.count; ++n) {
                struct v4l2_buffer buf;
                CLEAR (buf);
                buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                buf.memory      = V4L2_MEMORY_MMAP;
                buf.index       = n;

                if (ioctl (mVideoFd, VIDIOC_QUERYBUF, &buf)) {
                    WLOGE(myLog,  "VIDIOC_QUERYCAP fail\n");
                    goto err;
                }

                mVideoBuffer[n].length = buf.length;
                mVideoBuffer[n].start = mmap(NULL /* start anywhere */,
                                                 buf.length,
                                                 PROT_READ | PROT_WRITE /* required */,
                                                 MAP_SHARED /* recommended */,
                                                 mVideoFd, buf.m.offset);
                WLOGD(myLog, "UVC mmap %d: addr[%p] len[%d]", n, mVideoBuffer[n].start, mVideoBuffer[n].length);
                if (MAP_FAILED == mVideoBuffer[n].start) {
                    WLOGE(myLog,  "mmap fail\n");
                    goto err;
                }
            }

            break;
err:
            WLOGE(myLog, "USB Camera ioctl failed: %s", strerror(errno));
            closefd();
        }
    }

    if(mVideoFd < 0) {
    	WLOGE(myLog, "All Camera Node open failed");
        system("echo 1 > /sys/devices/virtual/misc/gpio_ctl/usbcam");
        usleep(3000000);
    	return -1;
    }

    WLOGI(myLog, "initDevice done mVideoFd = %d", mVideoFd);
  	return 0;
}

int USBCam::getBuffer(v4l2_buffer* buf)
{
    fd_set fds;
    struct timeval tv;
    int ret = 0;
    uint8_t* data = NULL;

retry:
    if(mVideoFd < 0) {
        WLOGW(myLog, "getBuffer mVideoFd %d reopen the camera device", mVideoFd);
        initDevice();
        if(mVideoFd >= 0)
            startStreaming();
        else
            return -1;
    }

    FD_ZERO (&fds);
    FD_SET (mVideoFd, &fds);
    tv.tv_sec = 2;
    tv.tv_usec = 0;
    
    ret = select(mVideoFd + 1, &fds, NULL, NULL, &tv);
    if(ret == 0) {
        WLOGE(myLog, "USB Camera select timeout: %s tv_sec %lds", strerror(errno), tv.tv_sec);
        goto error;
    } else if(ret == -1) {
        WLOGE(myLog, "USB Camera select error: %s", strerror(errno));
        goto error;
    }

    CLEAR(*buf);    
    buf->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf->memory = V4L2_MEMORY_MMAP;
    if (ioctl (mVideoFd, VIDIOC_DQBUF, buf)) {
        WLOGE(myLog, "USB Camera Dequeue Failed: %s", strerror(errno));
        goto error;
    }

    data = (uint8_t*)mVideoBuffer[buf->index].start;
    WLOGD(myLog, "UVC DQBUF Index[%d] Addr[%p]", buf->index, data);
    if(miFormat == V4L2_PIX_FMT_MJPEG) {
        if((data[0] != 0xFF) || (data[1] != 0xD8)) {
            WLOGE(myLog, "Get USB Display index %d, Data Error", buf->index);
            putBuffer(buf);
            goto retry;
        }
    }
    miFrameNumber++;
    
    mForcePowerDown = 0;
    return 0;
    
error:
    stopStreaming();
    closefd();

    if(mForcePowerDown ++ == 10) {
        mForcePowerDown = 0;
        WLOGE(myLog, "USBCam getBuffer Failed, Power Down and Retry");
        system("echo 1 > /sys/devices/virtual/misc/gpio_ctl/usbcam");
    }

    return -1;
}

void USBCam::putBuffer(v4l2_buffer* buf)
{
    WLOGD(myLog, "UVC QBUF Index[%d] Addr[%p]", buf->index, (uint8_t*)mVideoBuffer[buf->index].start);
    if (ioctl (mVideoFd, VIDIOC_QBUF, buf)) {
        WLOGE(myLog, "USB Camera Queue Failed: %s close %d", strerror(errno), mVideoFd);
        stopStreaming();
        closefd();
    }

    return;
}

const char* USBCam::getCameraType(int format)
{
    switch(format) {
        case V4L2_PIX_FMT_YUYV:
            return "YUYV";

        case V4L2_PIX_FMT_MJPEG:
            return "MJPG";
            
        case V4L2_PIX_FMT_H264:
            return "H264";
        default:
            WLOGE(myLog, "Error Camera Format: %d", format);
            return "Unknown";
    }
    
    return "Unknown";
}   

void USBCam::closefd()
{
    if(mVideoFd >= 0) {
        WLOGI(myLog, "close mVideoFd %d", mVideoFd);
        CLOSE_FD(mVideoFd);
    }
}
};