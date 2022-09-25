#include<iostream>
#include<thread> //引入 线程.... 似乎chrono的实现被移动到了 thread
//#include<chrono> //引入 timer


#define LOG(x) std::cout<<x<<std::endl //std::endl似乎有点慢??

static bool if_enter=false;

void DoWork()
{
	
	LOG("线程开!");
	while (1)
	{
		using namespace std::literals::chrono_literals; 
		//这是为了 1s 能被sleep_for理解,定义了操作符""s ,""h等
		LOG("线程2~:快点敲回车!!"); //如果你一直以最快速度干这些活,这个线程会把cpu占用率搞到100%
		std::this_thread::sleep_for(0.5s);
		std::this_thread::sleep_for(std::chrono::milliseconds(500)); //0.5秒

		//std::this_thread 当前进程的一些东西,用来控制当前进程
		//get_ID
		std::cout << "now the thread's id is: " << std::this_thread::get_id() << std::endl;
		if (if_enter) break;
	}
	
	LOG("线程将结束");
}

struct Timer
{
	std::chrono::steady_clock::time_point start ,end;
	std::chrono::duration<float> duration;  //nano是纳秒??
	Timer() {

		start = std::chrono::high_resolution_clock::now();
	}
	~Timer()
	{
		end = std::chrono::high_resolution_clock::now();
		duration = end - start; //  /s
		float durms = duration.count()*1000.0f;
		LOG("it live for " << durms << "ms");
	}

};

int main()
{
	auto start = std::chrono::high_resolution_clock::now();
	LOG("part42:线程");
	{

	//让计算机 同时执行多条指令 ,
	//在这之前我们的代码都是顺序执行的,一个std::cin.get()就能堵死,因为它会停下等待

	std::thread work(DoWork); //开启另一个线程,执行函数 线程name(函数名/指针)
	LOG("快点敲回车!!");
	// 	while (1)
	// 	{
	// 		LOG("线程1~");
	// 	}
	std::cin.get();
	if_enter = true; //在循环输出的同时等待输入些什么,这就用上线程了
	LOG("敲了!!");
	work.join();//线程join , 一个线程结束并回到当前线程时这行代码才会继续执行,即,等待线程结束
	LOG("线程已加入");
	std::cout << "now the thread's id is: " << std::this_thread::get_id() << std::endl;

	//你可以自己用线程实现一个 cin.get() 不会阻塞线程的那种

	//线程可以很复杂
	}

	LOG("part43:计时");
	{
		//std::chrono 为您准点报时~
		//chrono C++std提供的精确计时库 ,在此之前需要操作系统库来获得精确计时
		LOG("计时方式1");
		using namespace std::literals::chrono_literals;
		auto start=std::chrono::high_resolution_clock::now(); //
		std::this_thread::sleep_for(1s);
		auto end = std::chrono::high_resolution_clock::now();
		//这种计时方式会有本身开销,不是非常准确

		std::chrono::duration<float> durat = end - start;
		LOG(durat.count());

		{
			LOG("计时方式2");
			Timer time;
			std::this_thread::sleep_for(1s);
		}


	}

	LOG("part44:多维数组");
	{
		//多维其实是 让高维度数组元素变成低维度数组的指针,即高纬度数组的元素是低维度数组的指针
		int* array1 = new int[10]; //一个int 4字节 ,分配 40字节内存
		int** array2 = new int*[10]; // 一个* 4字节 , 分配 40字节内存
		//不要管你是什么类型的指针
		//记住,你只是分配了内存

		
		for (int i = 0; i < 10; i++)
			array2[i] = new int[10];

		//删除这样的数组有些麻烦, delete[] array2 只是删除了二级指针数组,40内存,甚至你失去了子数组的指针而没有删掉它会造成内存泄漏
		//想要删除整个需要遍历
		for (int i =0;i<10;i++)
		{
			delete[] array2[i];

		}
		delete[] array2;
		//还有内存碎片问题,由于堆上是随机分配,这10个数组不是连续的,当你需要跳来跳去变量这个二维数组时
		//有可能造成cache miss

		//更好的方式:假装是高维数组
		int* a2d = new int[10 * 10];
		for (int y=0;y<10;y++)
			for (int x = 0; x < 10; x++) {
				a2d[x + y * 10] = 2;
			}
		//也许你可以建个类然后重构一下 []方法

// 		int*** array3 = new int** [10];
// 		for (int i = 0; i < 10; i++)
// 		{
// 			array3[i] = new int* [10];
// 			for (int j = 0;j < 10;j++)
// 				array3[i][j] = new int[10];
// 			//动动笔拉大哥
// 		}

	}
}
