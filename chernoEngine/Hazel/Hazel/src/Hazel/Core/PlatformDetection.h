#pragma once


#ifdef _WIN32
#ifdef _WIN64
#define HZ_PLATFORM_WINDOWS
#else
#error "x86 Buidls are not supported"
#endif
#elif defined(__APPLE__)||defined(__MACH__)
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR ==1
#error "IOS simulator is not supported"
#elif TARGET_OS_IPHONE ==1
#define HZ_PLATFORM_IOS
#error "IOS is not supported!"
#elif TARGET_OS_MAC ==1
#define "MacOs is not supported!"
#else
#error "UnKonw Apple platform!"
#endif

#elif defined(__ANDROID__)
#define  HZ_PLATFORM_ANDROID
#error "Android is not supported!"
#elif defined(__Linux__)
#define HZ_PLATFORM_LINUX 
#error "Linux is not supported!"
#else
#error "unKown platform!"
#endif // _WIN32