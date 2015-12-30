LOCAL_PATH := $(call my-dir)

JNI_LOCAL_PATH := $(LOCAL_PATH)

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
    ../../Classes/InviteFriendsScene.cpp \
    ../../Classes/AdsTestScene.cpp \
    ../../Classes/VideosTestScene.cpp \
    ../../Classes/InterstitialsTestScene.cpp \
		../../Classes/ReportTestScene.cpp \
    ../../Classes/BannersTestScene.cpp \
    ../../Classes/MuffinRushAssets.cpp \
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
LOCAL_SRC_FILES += ../../Classes/Soomla//CCCoreBridge.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//CCCoreConsts.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//CCCoreEventDispatcher.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//CCDomainFactory.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//CCDomainHelper.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//CCError.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//CCJsonHelper.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//CCNdkBridge.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//CCSchedule.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//CCSimpleStoreEventHandler.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//CCSoomla.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//CCSoomlaEventDispatcher.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//CCSoomlaStore.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//CCSoomlaUtils.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//CCStoreBridge.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//CCStoreConsts.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//CCStoreEventDispatcher.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//CCStoreInventory.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//data/CCBridgelessKeyValueStorage.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//data/CCKeyValueStorage.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//data/CCRewardStorage.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//data/CCStoreInfo.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//data/CCVirtualCurrencyStorage.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//data/CCVirtualGoodsStorage.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//data/CCVirtualItemStorage.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//domain/CCDomain.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//domain/CCMarketItem.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//domain/CCPurchasableVirtualItem.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//domain/CCSoomlaEntity.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//domain/CCVirtualCategory.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//domain/CCVirtualItem.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//domain/virtualCurrencies/CCVirtualCurrency.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//domain/virtualCurrencies/CCVirtualCurrencyPack.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//domain/virtualGoods/CCEquippableVG.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//domain/virtualGoods/CCLifetimeVG.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//domain/virtualGoods/CCSingleUsePackVG.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//domain/virtualGoods/CCSingleUseVG.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//domain/virtualGoods/CCUpgradeVG.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//domain/virtualGoods/CCVirtualGood.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//jsb/jsb_soomla.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//jsb/JSBinding.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//NativeImpl/CCNativeCoreBridge.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//NativeImpl/CCNativeKeyValueStorage.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//NativeImpl/CCNativeRewardStorage.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//NativeImpl/CCNativeSoomlaStore.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//NativeImpl/CCNativeStoreBridge.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//NativeImpl/CCNativeStoreInfo.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//NativeImpl/CCNativeVirtualCurrencyStorage.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//NativeImpl/CCNativeVirtualGoodsStorage.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//PurchaseTypes/CCPurchaseType.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//PurchaseTypes/CCPurchaseWithMarket.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//PurchaseTypes/CCPurchaseWithVirtualItem.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//rewards/CCAggregateReward.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//rewards/CCBadgeReward.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//rewards/CCRandomReward.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//rewards/CCReward.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//rewards/CCSequenceReward.cpp
LOCAL_SRC_FILES += ../../Classes/Soomla//rewards/CCVirtualItemReward.cpp

LOCAL_SRC_FILES += ../../Classes/jansson/dump.c
LOCAL_SRC_FILES += ../../Classes/jansson/hashtable.c
LOCAL_SRC_FILES += ../../Classes/jansson/memory.c
LOCAL_SRC_FILES += ../../Classes/jansson/strbuffer.c
LOCAL_SRC_FILES += ../../Classes/jansson/strconv.c
LOCAL_SRC_FILES += ../../Classes/jansson/error.c
LOCAL_SRC_FILES += ../../Classes/jansson/load.c
LOCAL_SRC_FILES += ../../Classes/jansson/pack_unpack.c
LOCAL_SRC_FILES += ../../Classes/jansson/utf.c
LOCAL_SRC_FILES += ../../Classes/jansson/value.c

LOCAL_C_INCLUDES := $(JNI_LOCAL_PATH)/../../Classes
LOCAL_C_INCLUDES += $(JNI_LOCAL_PATH)/../../Classes/Soomla
LOCAL_C_INCLUDES += $(JNI_LOCAL_PATH)/../../Classes/jansson
LOCAL_C_INCLUDES += $(JNI_LOCAL_PATH)/../../AppWarpX
LOCAL_C_INCLUDES += $(JNI_LOCAL_PATH)/../../../PlayscapePubkitCocos2D-X/include
LOCAL_C_INCLUDES += $(JNI_LOCAL_PATH)/../../Classes/Soomla/data
LOCAL_C_INCLUDES += $(JNI_LOCAL_PATH)/../../Classes/Soomla/domain
LOCAL_C_INCLUDES += $(JNI_LOCAL_PATH)/../../Classes/Soomla/domain/virtualCurrencies
LOCAL_C_INCLUDES += $(JNI_LOCAL_PATH)/../../Classes/Soomla/domain/virtualGoods
LOCAL_C_INCLUDES += $(JNI_LOCAL_PATH)/../../Classes/Soomla/jsb
LOCAL_C_INCLUDES += $(JNI_LOCAL_PATH)/../../Classes/Soomla/PurchaseTypes
LOCAL_C_INCLUDES += $(JNI_LOCAL_PATH)/../../Classes/Soomla/NativeImpl
LOCAL_C_INCLUDES += $(JNI_LOCAL_PATH)/../../Classes/Soomla/rewards

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_curl_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static
#LOCAL_WHOLE_STATIC_LIBRARIES += jansson_static

# Playscape libs we're dependant on + gnustl shared lib
LOCAL_SHARED_LIBRARIES := playscape_pubkit_shared
LOCAL_SHARED_LIBRARIES += playscape_pubkit_cocos2dx_shared
LOCAL_SHARED_LIBRARIES += gnustl_shared

include $(BUILD_SHARED_LIBRARY)

# Required path, playscape publishing kit modules reside there.
$(call import-add-path,$(JNI_LOCAL_PATH)/../../../PlayscapePubkitCocos2D-X/proj.android/prebuilt)

# Playscape Publishing Kit Modules
$(call import-add-path,$(JNI_LOCAL_PATH))
$(call import-module,playscape_pubkit)
$(call import-module,playscape_pubkit_cocos2dx)

# Cocos2D-X Modules

$(call import-module,libcurl)
$(call import-module,CocosDenshion/android)
$(call import-module,cocos2dx)
#$(call import-module,external/jansson)