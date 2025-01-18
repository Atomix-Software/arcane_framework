#pragma once

#ifdef _WIN32
	#ifdef _WIN64
		#ifndef ARC_PLATFORM_WINDOWS
		    #define ARC_PLATFORM_WINDOWS
		#endif
	#else
		#error "x86 Builds are not supported!"
	#endif
#else
	#error "Other OS Builds are currently unsupported!"
#endif

#ifdef ARC_PLATFORM_WINDOWS
	#ifdef ARC_DYNAMIC_LINK
		#ifdef ARC_BUILD_DLL
			#define ARC_API __declspec(dllexport)
		#else
			#define ARC_API __declspec(dllimport)
		#endif
	#else
		#define ARC_API 
	#endif

    #ifdef _DEBUG
        #define ARC_DEBUG
    #else
        #define ARC_RELEASE
    #endif
#else
	#error We only support Windows!
#endif