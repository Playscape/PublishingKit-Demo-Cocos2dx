NDK_TOOLCHAIN_VERSION=4.8
APP_STL := gnustl_static
APP_CPPFLAGS := -frtti -DCC_ENABLE_CHIPMUNK_INTEGRATION=1 -DCOCOS2D_DEBUG=1 -std=c++11 -Wno-literal-suffix -fsigned-char
APP_CPPFLAGS += -fexceptions
APP_ABI := armeabi armeabi-v7a
