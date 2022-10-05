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

#ifdef HZ_DEBGUG
#define HZ_ENABLE_ASSERTS
#endif // HZ_DEBGUG



#ifdef HZ_ENABLE_ASSERTS
#define HZ_ASSERT(x,...) {if(!(x)) {HZ_EEROR("Assertion Failed:{0}",__VA_ARGS__ );__debugbreak();} }
#define HZ_CORE_ASSERT(x,...) {if(!(x)) {HZ_CORE_EEROR("Assertion Failed:{0}",__VA_ARGS__ );__debugbreak();} }
#else
#define HZ_ASSERT(x,...)
#define HZ_CORE_ASSERT(x,...)
#endif // HZ_ENABLE_ASSERTS


#define BIT(x) (1<<x) //1位移x位

#define  BIND_EVENT_CALLFN(x) std::bind(&x,this,std::placeholders::_1) //不是很懂& 和 this 干嘛?: 函数引用?,使用this,是成员函数的隐藏参数吧