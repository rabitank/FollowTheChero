#pragma once
//some basic macro
#include<memory>



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
#define GLCall(x) x
#endif // HZ_ENABLE_ASSERTS


#define BIT(x) (1<<x) //1位移x位

//不是很懂& 和 this 干嘛?: 函数引用?,使用this,是成员函数的隐藏参数吧
//记得 带上命名空间/类名
#define  BIND_EVENT_CALLFN(x) std::bind(&x,this,std::placeholders::_1) 

namespace Hazel
{
	template <typename T>
	using Scope = std::unique_ptr<T>;//指明作用域限定
	template<typename T ,typename... Args> //typename...  形参列表包 命名为 Args
	constexpr Scope<T> CreateScope(Args&& ... args)//Args引用折叠后 命名为 args(变为右值?)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);//转发形参args(...解包)
	}

	template <typename T>
	using Ref = std::shared_ptr<T>; //指明资产的ref性质,并对ref进行包装,方便之后的多线程通信,我们只需对自己的资产强调这一点

	template <typename T,typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)//constexpr func() 常量表达式函数修饰....保证返回常量(只能有return和其他几种命令),会在编译时计算好
	{
		return std::make_shared<T>(std::forward<Args>(args)...); 

	}

}