LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= OpenGLTriangle.cpp

LOCAL_SHARED_LIBRARIES := \
    libcutils \
    liblog \
    libEGL \
    libGLESv2 \
    libui \
    libgui \
    libutils

LOCAL_STATIC_LIBRARIES += libglTest

LOCAL_C_INCLUDES += $(call include-path-for, opengl-tests-includes)

#LOCAL_MODULE:= OpenGLTriangle opengl-triangle
LOCAL_MODULE:= opengl-triangle

LOCAL_MODULE_TAGS := optional

LOCAL_CFLAGS := -DGL_GLEXT_PROTOTYPES

include $(BUILD_EXECUTABLE)
