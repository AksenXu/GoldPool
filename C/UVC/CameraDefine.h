#ifndef _PL_CAMERA_DEFINE_H_
#define _PL_CAMERA_DEFINE_H_

/* This needs to be increased if we can have more cameras */
#define MAX_CAMERAS 5

//set in NetlinkHandler, and use in DVR2 and CameraService
#define USB_CAMERA_PROPERTY_BACK    "sys.usb.camnode"
#define USB_CAMERA_PROPERTY_LEFT    "sys.usb.camnode.l"
#define USB_CAMERA_PROPERTY_RIGHT   "sys.usb.camnode.r"

#define USB_CAMERA_BACK     2
#define USB_CAMERA_LETT     3
#define USB_CAMERA_RIGHT    4

enum CAMERA_TYPE {
    MIPI1_CAMERA,
    MIPI2_CAMERA,
    USB_CAMERA1 = USB_CAMERA_BACK,
    USB_CAMERA2 = USB_CAMERA_LETT,
    USB_CAMERA3 = USB_CAMERA_RIGHT,
    FAKE_CAMERA
};

#endif