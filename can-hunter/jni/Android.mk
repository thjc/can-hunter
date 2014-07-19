LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

include ../../../android/opencv/OpenCV-2.4.8-android-sdk/sdk/native/jni/OpenCV.mk

LOCAL_MODULE    := can_hunter_jni
LOCAL_SRC_FILES := jni_part.cpp MotorControl.cpp
LOCAL_LDLIBS +=  -llog -ldl

include $(BUILD_SHARED_LIBRARY)
