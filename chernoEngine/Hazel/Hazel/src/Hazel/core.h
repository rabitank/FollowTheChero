#pragma once
//some basic macro


#ifdef HZ_PLATFORM_WINDOWS
	#ifdef HZ_BUILD_DLL
		#define HAZEL_API __declspec(dllexport)
	#else
		#define HAZEL_API __declspec(dllimport)
	#endif
#else
	#error Hazel only support windows!
#endif

#ifdef HZ_ENABLE_ASSERTS
#define HZ_ASSERT(x,...) {if(!(x)) {HZ_EEROR("Assertion Failed:{0}",__VA_ARGS__ );__debugbreak();} }
#define HZ_CORE_ASSERT(x,...) {if(!(x)) {HZ_CORE_EEROR("Assertion Failed:{0}",__VA_ARGS__ );__debugbreak();} }
#else
#define HZ_ASSERT(x,...)
#define HZ_CORE_ASSERT(x,...)
#endif // HZ_ENABLE_ASSERTS


#define BIT(x) (1<<x) //1Î»ÒÆxÎ»