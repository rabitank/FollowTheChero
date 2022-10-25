#pragma once
//some basic macro
#include<memory>


#ifdef HZ_PLATFORM_WINDOWS
#if HZ_DYNAMIC_LINK
	#ifdef HZ_BUILD_DLL
		#define HAZEL_API __declspec(dllexport)
	#else
		#define HAZEL_API __declspec(dllimport)
	#endif
#else
	#define HAZEL_API 
#endif	

#else
	#error Hazel only support windows!
#endif

#ifdef HZ_DEBUG
#define HZ_ENABLE_ASSERTS
#endif // HZ_DEBGUG



#ifdef HZ_ENABLE_ASSERTS
#define HZ_ASSERT(x,...) {if(!(x)) {HZ_ERROR("Assertion Failed:{0}",__VA_ARGS__ );__debugbreak();} }
#define HZ_CORE_ASSERT(x,...) {if(!(x)) {HZ_CORE_ERROR("Assertion Failed:{0}",__VA_ARGS__ );__debugbreak();} }

void GlClearError();
bool GlLogCall(const char* function, const char* file, int line);

#define GLCall(x) GlClearError();x;HZ_CORE_ASSERT(GlLogCall(#x,__FILE__,__LINE__)," OpenGL Error! "); //#x ,把x变为字符串,后两个是所有编译器支持的
#else
#define HZ_ASSERT(x,...)
#define HZ_CORE_ASSERT(x,...)
#endif // HZ_ENABLE_ASSERTS


#define BIT(x) (1<<x) //1位移x位

#define  BIND_EVENT_CALLFN(x) std::bind(&x,this,std::placeholders::_1) //不是很懂& 和 this 干嘛?: 函数引用?,使用this,是成员函数的隐藏参数吧

namespace Hazel
{
	template <typename T>
	using Scope = std::unique_ptr<T>;//指明作用域限定

	template <typename T>
	using Ref = std::shared_ptr<T>; //指明资产的ref性质,我们只需对自己的资产强调这一点


}