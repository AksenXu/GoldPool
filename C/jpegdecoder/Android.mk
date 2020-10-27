LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_C_INCLUDES :=	PL_Common/PL_Inc/ \
                    device/sprd/sharkle/sl8521e_3h10_dvr_hs/prj_cm \
                    external/skia/include/core \
                    external/skia/src/images/ \
                    external/jpeg
                    
                    
LOCAL_SHARED_LIBRARIES := libcarutils libnativecomm libskia libjpeg
LOCAL_SRC_FILES := main.cpp JpegDecoder.cpp
LOCAL_MODULE_TAGS := user
LOCAL_MODULE    := jpegdecoder
include $(BUILD_EXECUTABLE)