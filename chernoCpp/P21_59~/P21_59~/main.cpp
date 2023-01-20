#include<iostream>
#define LOG(x) std::cout<<x<<std::endl
#include <future> //多线程 与 异步加载 与 锁
#include <mutex>
#include <string_view>

uint32_t s_AllocCount = 0;

void* operator new(size_t size) //让new显式化 , 全局的new(返回空类型指针) 或者说是c++原生的new,只能接收size_t -> stl使用
{
	s_AllocCount++;
	std::cout << "Allocating : "<<size<< std::endl;
	return malloc(size);
}

void Printname(const std::string& name)
{
	std::cout << name<<std::endl;
}
void Printname(const std::string_view& name)
{
	std::cout << name<<std::endl;
}

int main()
{
//LOG("part59:c++更快的多线程加载优化");//没看明白,多核异步处理 async(),多核提交mesh时,为了防止并发访问vector用了lock/互斥锁?

	LOG("part60:c++更快的字符操作");
	{
#if 0
			std::string name = "ilove OpenGL";
			const char* tag = "MyHero Cherno";
	#if 0
			std::string first = name.substr(0, 5);
			std::string last = name.substr(6, 12);
			//我们只是想查看这段内存中的一部分,而不是在堆中新建,并且string老是往堆跑也不是事.
	#else
			//使用c++17:  string_view  一个const char*(sub开头) 和 一个 size , 指向现有字符串的部分 , 不会分配新内存 , 
	//也有手动方法
			std::string_view first(name.c_str(), 5);
			std::string_view last(name.c_str() + 6, 6);
	#endif

#else
			const char* tag = "MyHero Cherno";
			// const char*  的形式同时也很亲和c风格的字符串
			std::string_view first(tag, 6);
			std::string_view last(tag+7, 6);

#endif





		//Printname("OpenGl"); //就算是const & 面对不同类型还是得构建一次
		//Printname(name);
		Printname(first);
		Printname(last);
		//Printname(name.substr(0, 12));//name.substr 本身构建一次std::string

		//PS:visual studio（msvc）的debug模式应该是为了插入调试符号进行了new调用，字符串本身应该也是处于栈分配的。如果是release模式有短字符串优化SSO
			//短字符串优化SSO:gcc的string默认大小是32个字节，字符串小于等于15直接保存在栈上，超过之后才会使用new分配.gcc的string默认是sso的。

		std::cout << s_AllocCount << " Allocated" << std::endl;
	}
}