LOCAL_PATH := $(call my-dir)

define all-c-cpp-files-under5
$(patsubst ./%,%, \
  $(shell cd $(LOCAL_PATH) ; \
          find -L $(1) -maxdepth 5 \( -name "*.c" -or -name "*.cpp" -or -name "*.cc" \) -and -not -name ".*") \
 )
endef

include $(CLEAR_VARS)
LOCAL_MODULE    := libcurl_custom
LOCAL_SRC_FILES := libcurl_custom.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := libcrypto
LOCAL_SRC_FILES := libcrypto.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := liboss-cpp-sdk

LOCAL_CFLAGS += -Wall -Wno-unused-parameter -DANDROID_NDK -DJSONCPP_NO_LOCALE_SUPPORT

LOCAL_MODULE_TAGS := optional
#LOCAL_CPP_EXTENSION := .cc

LOCAL_SRC_FILES := $(call all-c-cpp-files-under5, .)
$(info $(LOCAL_SRC_FILES))

LOCAL_C_INCLUDES := bionic \
                    $(LOCAL_PATH)/include     \
                    $(LOCAL_PATH)/include/alibabacloud/oss   \
                    $(LOCAL_PATH)/src/external  \
                    external/stlport/stlport    \
                    /media/data/SPRD/SL8521/trunk/PL_Common/curl-7.43.0/include/ \
                    /media/data/SPRD/SL8521/trunk/external/openssl/include

LOCAL_STATIC_LIBRARIES :=
LOCAL_SHARED_LIBRARIES := libcrypto libcurl_custom
                            
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := oss-ptest

LOCAL_CFLAGS += -Wall -Wno-unused-parameter -DANDROID_NDK -DJSONCPP_NO_LOCALE_SUPPORT

LOCAL_MODULE_TAGS := optional

LOCAL_SRC_FILES := $(call all-c-cpp-files-under5, $(LOCAL_PATH)/../ptest)
$(info $(LOCAL_SRC_FILES))

LOCAL_C_INCLUDES := bionic \
                    $(LOCAL_PATH)/include     \
                    $(LOCAL_PATH)/include/alibabacloud/oss   \
                    $(LOCAL_PATH)/src/external  \
                    external/stlport/stlport    \
                    /media/data/SPRD/SL8521/trunk/PL_Common/curl-7.43.0/include/ \
                    /media/data/SPRD/SL8521/trunk/external/openssl/include

LOCAL_STATIC_LIBRARIES :=
LOCAL_SHARED_LIBRARIES := libcrypto libcurl_custom liboss-cpp-sdk
                            
include $(BUILD_EXECUTABLE)


include $(CLEAR_VARS)
LOCAL_MODULE := oss-sample

LOCAL_CFLAGS += -Wall -Wno-unused-parameter -DANDROID_NDK -DJSONCPP_NO_LOCALE_SUPPORT

LOCAL_MODULE_TAGS := optional

LOCAL_SRC_FILES := $(call all-c-cpp-files-under5, $(LOCAL_PATH)/../sample)
$(info $(LOCAL_SRC_FILES))

LOCAL_C_INCLUDES := bionic \
                    $(LOCAL_PATH)/include     \
                    $(LOCAL_PATH)/include/alibabacloud/oss   \
                    $(LOCAL_PATH)/src/external  \
                    external/stlport/stlport    \
                    /media/data/SPRD/SL8521/trunk/PL_Common/curl-7.43.0/include/ \
                    /media/data/SPRD/SL8521/trunk/external/openssl/include

LOCAL_STATIC_LIBRARIES :=
LOCAL_SHARED_LIBRARIES := libcrypto libcurl_custom liboss-cpp-sdk
                            
include $(BUILD_EXECUTABLE)


include $(CLEAR_VARS)
LOCAL_MODULE := oss-test

LOCAL_CFLAGS += -Wall -Wno-unused-parameter -DANDROID_NDK -DJSONCPP_NO_LOCALE_SUPPORT

LOCAL_MODULE_TAGS := optional

LOCAL_SRC_FILES := $(call all-c-cpp-files-under5, $(LOCAL_PATH)/../test)
$(info $(LOCAL_SRC_FILES))

LOCAL_C_INCLUDES := bionic \
                    $(LOCAL_PATH)/include     \
                    $(LOCAL_PATH)/include/alibabacloud/oss   \
                    $(LOCAL_PATH)/src/external  \
                    $(LOCAL_PATH)/../test/external  \
                    external/stlport/stlport    \
                    /media/data/SPRD/SL8521/trunk/PL_Common/curl-7.43.0/include/ \
                    /media/data/SPRD/SL8521/trunk/external/openssl/include

LOCAL_STATIC_LIBRARIES :=
LOCAL_SHARED_LIBRARIES := libcrypto libcurl_custom liboss-cpp-sdk
                            
include $(BUILD_EXECUTABLE)