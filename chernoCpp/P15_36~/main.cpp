#include<iostream>
#include<vector>
#include<array>
#include<functional>//for lambda
#include<algorithm> //for find_if
//#define WAIT std::cin.get()
//不要这么写,可读性会变差

#define LOG(x) std::cout<<x<<std::endl
//可能用法

//2 :利用宏在release版本中去掉日志系统
//在 项目 - 属性 - C++ - 预处理器 - 预处理定义 中决定定义哪些词(不能有空格) -> 代码中为 #define 你的输入
//如果没有定义P15_DEBUG , P15_DEBUG==1仍然可以被处理并认为是false
//另外 0 同样在预处理中被认为是false
#if 0

#define P15_RELEASE=0
#if P15_DEBUG ==1
#define  LOGX(x) std::cout<<x<<std::endl
#else
#define LOGX(x) 
#endif // P15_DEBUG

#endif

//多行宏 不要这么干 用\转义掉换行符
// 
// #define MAIN int main() \
// {\
// 	LOG("MAIN函数在宏");\
// }
// 
// MAIN

std::string getname()
{

	return "rabitank";
}
auto getsmt() {
	return "hh";
}

template<typename T>
void printArray(const T& array)
{
	std::cout << array.size() << std::endl;
}

void Printhello( int a) {
	LOG("hellow world");
	LOG(a);
}

void fuckvalue(int a)
{
	std::cout << a << " had been fucked" << std::endl;
}
void Foreach( std::vector<int > values , void(*whatyouwantdo)(int) )
{
	for (int value : values)
	{
		whatyouwantdo(value);
	}
}
void Foreachlambda(std::vector<int > values, const std::function<void(int)>& whatyouwantdo)
{
	for (int value : values)
	{
		whatyouwantdo(value);
	}
}

namespace sb { namespace function {
		
	static int sbInTheWorld = 2;
} }
//namespace SB = sb;


class rabit
{
	private:
		int a, b;
public:
	static void printType() {
		std::cout << "rabit" << std::endl;
	}
	static std::array<int, 4>& arr;
	//我们不能把静态成员的初始化放置在类的定义中，但是可以在类的外部通过使用范围解析运算符 :: 来重新声明静态变量从而对它进行初始化
};
int main()
{

	std::cout << "part36:宏_进阶" << std::endl;
	{
		//WAIT;
		//预编译器,纯文本替换
		LOG("我爱C++宏");
		//LOGX("我讨厌C++宏!");
	}
	LOG(5);
	LOG("part37:auto");
	{
		//编译器可以推导出类型时,使用auto自动规定类型
		auto a = 5;
		auto b = a;
		//他的好处是,当你改变自建函数的返回值时,使用auto让你不需要改动左值
		//但是对你的左值类型有要求时,你最好不要使用auto,让你的类型轻易改变
		//你不知道返回值是个啥时,也可以用auto
		//auto也可以用在 函数返回类型上
		//还有一种情况是,你的类型又臭又长,使用auto让你少打点字
		//或者 using yourname = typename 重新命名
		auto name = getname();
		std::vector<std::string> ss;
		ss.reserve(2);
		ss.push_back("sb");
		ss.push_back("cnm");
		for (auto it = ss.begin(); it != ss.end(); it++)
		{
			LOG(*it);
		}
		//auto可以加上const & * ... auto不会处理引用,它最多是复制
		int c = 4;
		const auto& staticptr = c;

		//反正,,,cherno不喜欢auto,would never use auto,他说会让代码难读


	}

	LOG("part38:std::array");
	{
		//std::array 静态数组(不会增长空间)
		std::array<int, 4> sa = {1,2,3,4}; 
		sa[2] = 34;
		//需要规定类型与长度 因为std中使用了模板 ,这其实是一种类类型
		//所以,面对可能出现的任意类型的实参,函数怎么传参? 使用模板写函数
		printArray(sa);
		//stdarray 支持迭代器 你可以 begin end foreach
		//stdarray 在debug模式中,array中有宏会有越界检测,你不能越界
		//std::array 并没有实质存储size , 返回的size()实际上是模板参数,类似宏,一个源码常数
		//
		//类比std::vector vector 是储存在堆上的底层数据(以我为准)
		//模板类 可以在直接写在头文件而不需要实现文件,因为它不存在
	}

	LOG("part39:c风格函数指针");
	{
		//函数就是一堆cpu指令 有入口,有返回,存储在内存的某个地方
		//你可以获得入口指令的指针,*表示解引用,()表示作为函数执行,
		auto hello = Printhello; 
		//*Printhello , &Printhello , 返回都和Printhello一样,是函数指针(有隐式转换) , () 号会将指针隐式转换为ab(*cd)() 调用;
		(*hello)(555);
		hello(666);
		//类型? 返回类型(*name)(函数参数类型);
		void(*sb)(int a) = Printhello;
		sb(777);

		typedef void(*hellofunction)(int a) ; //这里有些特殊 意为 typedef void(*name)()  ; name  inputname -> void(*inputname)() 
		hellofunction functo = Printhello;
		functo(888);

		//你可以把函数作为可变变量,在调用时传进另一个函数了,而不是找到定义写死;
		//在调用时将函数指针作为参数控制另一个函数的行为;附带执行一个函数? ,有点像 回调? .callback()?
		
		std::vector<int> vi = { 1,1,4,5,1,4 };
		Foreach(vi, fuckvalue);

		//lambda , 普通暂时(用完二进制指令会被删掉)匿名函数 [捕获符号:传入传出参数方式](形式参数){函数定义}
		Foreach(vi, [](int value) {std::cout << "value:" << value << std::endl; });

		//c语言风格已经没人用了,太怪了
	}
	LOG("part40:lambda");
	{
		//lambda 是我们不需要函数定义就可以使用一个函数的方法;
		//在使用函数指针的地方都能用lambda

		std::vector<int> vi = { 1,1,4,5,1,4 };

		//lambda , 普通暂时(用完二进制指令会被删掉)匿名函数 [捕获符号:传入传出参数方式](形式参数){函数定义}
		//函数未来的代码,就由lambda来决定!
		auto lambda = [](int value) {std::cout << "value:" << value << std::endl; };
		Foreach(vi, lambda);

		//捕获 获得lambda外部局部变量,传递到lambda body中供你使用
		//无限扩展可传参数????!!!! C++nb嗷
		int i = 3;
		const char* string = "个杀马特团长!!!";
		auto pritnsmtnum = [&,i](int value) {std::cout << "value:" << value<<i<<string << std::endl; };
		Foreachlambda(vi, pritnsmtnum);
		// = 传递值(复制) , & 引用变量传递 , name 复制指定变量的值传递 , &name 引用特定变量传递 ,
		//传统 c风格的函数指针 与 c++ 有捕获的lambda 函数不兼容,被认为类型不同
		//c++ 无捕获的lambda 函数可以被隐式转换为函数指针
		//要使用有捕获lambda作为指针 需要引入 functional头文件
		
		//lambda形参 const std::function<void(int)>&   <返回类型(参数列表)>
		//lambda一般不允许你修改捕获的外部变量 即使是 [=]
		auto changenum = [&, i](int value) mutable {i = 2; vi.clear(); };
		//但你可以加入 mutable 允许修改

		//find_if 函数 根据是否满足某种条件返回的bool,返回在满足条件的第一个元素的位置的迭代器
		auto it =std::find_if(vi.begin(), vi.end(), [](int value) {return value > 4; });
		//在find_if 定义中看到 会调用函数并将迭代器的解引用(即元素)作为参数传入,所以lambda的形参类型得和元素类型相符
		LOG(*it);
	}

	LOG("part_std: why not using namespace std");
	{
		//你可以在局部作用域里用
		using namespace std;
		//但是cherno说:不用,至少不用using namespace std和eastl

		//1.std::明确类型或函数来自哪里,cherno有用eastl(开源c++库)代替std,这两类型名字一样,不好区分
		//2.std里一大堆东西可能用的不多.
		//3.同函数声明的不同函数在不同namespace被释放出来后会重复定义,即使形参类型不同变成了重载,根据优先匹配精确类型的重载原则,你还得判断调用了哪个函数
		//4.attention! 绝对不要在头文件里using namespace ,你会坑死后面的代码
		//5.在一个较小的作用域用是ok的
	}

	LOG("part41:namespace");
	{
		//只是防止名字冲突,并指名哪个库;
		//命名空间可以嵌套,续接
		//不能在作用域内定义
		
		// :: 进入命名空间 , 类等等也是一种命名空间 , 你需要 ::调用内部静态方法
		//使用类的命名空间构建静态变量来共享,跨单元传递重要常值/重要方法,你还可直接用命名空间进行同样操作
		rabit::printType(); //rabit.printType() 牡蛎
		rabit::arr[0] = 1; 
		std::cout << sb::function::sbInTheWorld << std::endl;
		//静态成员在类的所有对象中是共享的。如果不存在其他的初始化语句，在创建第一个对象时，所有的静态数据都会被初始化为零。
		//我们不能把静态成员的初始化放置在类的定义中，但是可以在类的外部通过使用范围解析运算符 :: 来重新声明静态变量从而对它进行初始化，

		//你还可以指定引入一个命名空间的某些东西
		using sb::function::sbInTheWorld;
		std::cout << sbInTheWorld << std::endl;

		//你还可以为命名空间起别名,作用域大小取决于你写在了哪里
		namespace SB = sb;
		std::cout << SB::function::sbInTheWorld << std::endl;
	
		//还可以引入子命名空间
		using namespace SB;
		using namespace function;
		//或
		using namespace SB::function;

	}
}
