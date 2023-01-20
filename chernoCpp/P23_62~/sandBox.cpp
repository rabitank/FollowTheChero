#include<iostream>
#include <Random>

class  Random_SingletonTest
{
public:
	Random_SingletonTest(const Random_SingletonTest& other) = delete; //非引用的赋值会导致单例复制。 -> 删除复制构造函数

	static Random_SingletonTest& Get()
	{
		static Random_SingletonTest s_Instance; 
		//静态成员需要在外部初始化.当然,作为函数内容在外部调用也可以,即第一次Get时才实例化 
		//任何静态变量都只会实例化一次,你多次Get也只会初始化一次

		//Java中有个静态代码块概念, 类域中只运行一次 ,放构造里就是第一次实例化才会执行,之后就不执行. 相当于扩展了static范围

		return s_Instance;
	}; //访问单例的接口


	static float Uint64()  //单例模式同样也可以兼容静态模式,但是需要多一层静态方法的包装, 省掉::Get()  -> 多包装,遇到机制要能绕过去-> 称为删掉一层交互
		//同时使用单例和静态模式不好, 使用起来会乱, 如果通过单例达到静态就把Get扔到private里)
	{
		return Get().IRandomUI64();
	}

private:
	 Random_SingletonTest(); //单例不允许在外部实例化

	 float m_RandomGenerator= 0.5f;


	 std::random_device m_devie;
	 std::mt19937_64 m_random;
	
	 float IFloat() {}; //I: internal 标记为私有方法
	 unsigned long long IRandomUI64()
	 {
		 return m_random();
	 }; //正常的方法
	 static Random_SingletonTest s_Instance;//单例本体


};




 Random_SingletonTest:: Random_SingletonTest()
{
	 m_random = std::mt19937_64(m_devie());
}



 struct AllocationMetrics
 {
	 uint32_t TotalAllocated = 0;
	 uint32_t TotalFreeed = 0;
	
	 uint32_t CurrentUsage() { return TotalAllocated - TotalFreeed; }
 
 };

 static AllocationMetrics s_allocationMetrics;


 struct vec3
 {
	 int x, y, z;
 };

	 void* operator new(size_t size)
		 //重载分配堆内存的全局作用域 ::operator new , 比较原始的底层new ,基本就是分配堆内存的包装而跟类型无关.
		 //所有涉及到堆内存分配的操作都会经过这个new
		 //没有重载前似乎使用的是外部二进制函数库实现的,只能看到声明
	 {
		 std::cout << "Allocating " << size << " bytes\n";
		 s_allocationMetrics.TotalAllocated += size;

		 return malloc(size);
	 }

	 void operator delete(void* memoery,size_t size ) 
		 // 不需要size ,但是可以通过添加这个参数获取信息 , 真是神奇啊(其实理论上是需要的...,光一个void指针,他可以较为智能地删掉指向类型大小(sizeof(class))的内存,但是对于分配时就是void*的内存他就不知道了)
	 {
		 std::cout << "Freeing  " << size << " bytes\n";
		 s_allocationMetrics.TotalFreeed += size;
		 return free(memoery);
	 }
 

	 static void PrintMemoryUsage()
	 {	
		 std::cout << "Memory Usage: " << s_allocationMetrics.CurrentUsage() << std::endl;
	 }
int main()
{
	PrintMemoryUsage();

	std::cout << "P62:单例模式(P82)" << std::endl;
	{
		//cherno认为单例的特称和命名空间很类似,他是一种静态方法于全局变量的组织方式
		//主要是,他不允许用户自己实例化对象,严格控制对象实例,只允许一个公用实例

		//单例化好处.或者说相对于直接用命名空间的优点是,能够隐藏功能实现,有选择地暴露方法
		//把要暴露的方法声明到.h文件中,cpp实现也是一种途径
		Random_SingletonTest& s_single = Random_SingletonTest::Get();
		auto& s_single_2 = Random_SingletonTest::Get();
		//or
		uint64_t randomNumber = Random_SingletonTest::Uint64();

		std::cout << randomNumber << std::endl;

		#if 0
				//but
				auto single = Singleton::Get();
				Singleton anotherSingle = Singleton::Get(); //非引用的赋值会导致单例复制。 -> 删除复制构造函数
		#endif
	}

	std::cout << "P63:小字符串优化(P82)" << std::endl;
	//就是之前的SSO
	//cherno给你看源码分析的
	// 
	//调用堆栈 看到 外部调用 ,想要分析底层可以右键展开

	std::cout << "P64:跟踪内存分配(P83)" << std::endl;
	//重载new
	{

		std::string string = "Rabitank";
		auto l_vec3 = new vec3{1,2,3};



		std::unique_ptr<vec3> l_vec3p = std::make_unique<vec3>();
		std::cout << l_vec3p->x << std::endl;

		PrintMemoryUsage();

		void* aBitMemory = ::operator new(32); // 直接调用,千万不要这样
		int* sup = (int*)aBitMemory;
		// ::特指全局作用域(前面啥也不加)

		delete[] sup,32;//woc,我写的什么jb , 对于数组分配,那是另一个delete[],并没有被跟踪,但是确实可以加size删掉的
		//声明:void __CRTDECL operator delete[](		void* _Block,	size_t _Size) noexcept;
		//但是这个操作符加参数的格式这样也行??

		delete l_vec3;
		// 	size_t size = 15;
		// 	int* x  =  new int(size);
		// 	std::cout << "x: "<<*x << std::endl;

	}

}