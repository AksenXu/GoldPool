LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := uvc_test
LOCAL_CFLAGS +=
LOCAL_MODULE_TAGS := optional
LOCAL_SRC_FILES := \
            usbcarmera_test.cpp \
            JpegUSBCam.cpp \
            USBCam.cpp
            
LOCAL_C_INCLUDES := \
            PL_Common/PL_Inc/ \

LOCAL_STATIC_LIBRARIES := libcarutils libcutils liblog
LOCAL_SHARED_LIBRARIES := 
include $(BUILD_EXECUTABLE)
