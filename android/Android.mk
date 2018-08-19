# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
#LOCAL_PATH := $(call my-dir)
#LOCAL_PATH := $(call my-dir)
#include $(CLEAR_VARS)

#LOCAL_ARM_NEON := true
#LOCAL_SHARED_LIBRARIES := gzstream
#LOCAL_MODULE := gzstream
#LOCAL_SRC_FILES := include/gzstream/gzstream.cpp
#LOCAL_C_INCLUDES += $(LOCAL_PATH)jni/include/gzstream
#LOCAL_LDFLAGS := -fPIE -lz
#LOCAL_CFLAGS := -fPIC
#include $(BUILD_SHARED_LIBRARY)

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := gzstream
LOCAL_SRC_FILES := libgzstream.so
include $(PREBUILT_SHARED_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE := ansvif
LOCAL_ARM_NEON := true
system_libs := stdc++ pthread z
APP_ALLOW_MISSING_DEPS=false
LOCAL_SHARED_LIBRARIES := gzstream
LOCAL_SRC_FILES := src/log.cpp src/bin2hex.cpp src/popen2.cpp src/main.cpp src/help.cpp src/match_fault.cpp src/sys_string.cpp src/man_read.cpp src/trash.cpp src/randomizer.cpp src/remove_chars.cpp src/to_int.cpp src/file_check.cpp src/reaper.cpp src/write_file.cpp src/templates.cpp
LOCAL_C_INCLUDES += $(LOCAL_PATH)jni/include/gzstream $(LOCAL_PATH)jni/include/xmlwriter $(LOCAL_PATH)jni/include
LOCAL_CFLAGS := -std=gnu++11 -mfloat-abi=softfp -D__ANDROID__ -D__linux -fPIE
LOCAL_CPP_FEATURES += exceptions rtti
LOCAL_LDLIBS += -lstdc++
LOCAL_LDFLAGS += -fPIE
include $(BUILD_EXECUTABLE)

