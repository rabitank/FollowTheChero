#include<iostream>
#define LOG(x) std::cout<<x<<std::endl
#include <future> //多线程 与 异步加载 与 锁
#include <mutex>

int main()
{
	LOG("part59:c++更快的多线程加载优化");//没看明白,多核异步处理 async(),多核提交mesh时,为了防止并发访问vector用了lock/互斥锁?
}