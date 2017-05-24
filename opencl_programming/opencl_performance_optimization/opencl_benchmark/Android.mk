LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := opencl.cpp main.cpp 

LOCAL_MODULE := opencl_benchmark

LOCAL_SHARED_LIBRARIES := libcutils libutils libOpenCL
LOCAL_MODULE_TAGS := eng optional tests
LOCAL_ARM_MODE := arm
#LOCAL_CFLAGS := -Werror
OPENCL_HEADER_PATH := $(LOCAL_PATH)/khronos/original
LOCAL_C_INCLUDES := $(TOP)/system/core/include/cutils/\
			$(LOCAL_PATH)/lib

# Mark source files as dependent on Android.mk
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
include $(BUILD_EXECUTABLE)
