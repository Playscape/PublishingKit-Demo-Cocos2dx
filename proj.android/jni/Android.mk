LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := game_shared

LOCAL_MODULE_FILENAME := libgame

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/HelloWorldScene.cpp \
                   ../../Classes/Player.cpp \
                   ../../Classes/StartGameLayer.cpp \
                   ../../Classes/InGameMenuLayer.cpp \
                   ../../Classes/StoreScene.cpp \
                   ../../AppWarpX/appwarp.cpp \
                   ../../AppWarpX/appwarp_extended.cpp \
                   ../../AppWarpX/base64.cpp \
                   ../../AppWarpX/HMAC_SHA1.cpp \
                   ../../AppWarpX/requests.cpp \
                   ../../AppWarpX/SHA1.cpp \
                   ../../AppWarpX/socket.cpp \
                   ../../AppWarpX/udpsocket.cpp \
                   ../../AppWarpX/urlencode.cpp \
                   ../../AppWarpX/utilities.cpp \
                   ../../AppWarpX/cJSON.c
                   
                   
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes $(LOCAL_PATH)/../../AppWarpX

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static cocosdenshion_static cocos_curl_static cocos_extension_static
            
include $(BUILD_SHARED_LIBRARY)

$(call import-module,libcurl)
$(call import-module,CocosDenshion/android)
$(call import-module,cocos2dx)