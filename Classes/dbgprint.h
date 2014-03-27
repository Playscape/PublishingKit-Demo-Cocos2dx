#ifndef __dbgprint_h__
#define __dbgprint_h__

	#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		#include <android/log.h>
		#define dbgprint(...) __android_log_print(ANDROID_LOG_INFO, "ps_native", __VA_ARGS__);
	#else
		#define dbgprint(...) printf(__VA_ARGS__);printf("\n");
	#endif

#endif
