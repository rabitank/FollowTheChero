
#include <iostream>
#include <tuple> //tuple
#include <functional> // make_pair()


// void print(const char* string) {
// 	std::cout << string << std::endl;
// }
// void print(int x) {
// 	std::cout << x << std::endl;
// }
// void print(float x) {
// 	std::cout << x << std::endl;
// }

//确实类似于宏
template<typename T> //我们选用typename 作为模板参数类型(getxtypename),T作为名字 这里class与typename同义 ,你也可以写成<class T>
void print(T value)
{
	std::cout << value << std::endl;
}

//template用于类上
template<typename T,int N> //T 是type,这里的 N 就是一个宏啊
class array
{
private:
	T m_array[N];
public:
	int getSize() {
		return N;
	}

};


std::string getString() {

	std::string s = "smttz";
	return s;
}
int main()
{
	std::cout << "part33::多返回值处理方法" << std::endl;
	{
		
		//1:输入参数指针/引用的多返回值处理方法,对外部变量进行更改 the cherno: 在目标参数加上out前缀,这样你知道改变(输出)的是哪些变量
		//2:传多返回值如多个字符串 可以 return new std::string[] {string1,string2 }; //但你不知道它多大
		//3:返回std::array ; 这里的return似乎可以帮助留下return内容作为栈的暂时返回值,不是必须要new才能留存到复制的时候
		//	auto s = getString();
		//std::cout << s << std::endl;
		//4:返回std::vector 比array差一点


		//通用办法:即使是不同类型的返回值也能一起返回
		{
			//tuple     pair
			//tuple 元组 ; 它可以认为是类,包含n个变量但不关心其类型
			std::tuple<std::string, std::string, int> tuplessi((std::string)"smttz", (std::string)"wgnpl", 9);
			//不允许使用不完整的类型:可能之一,你没有这个类型的定义,只有声明
			//pair make_pair 返回pair , 只能包含两个变量,不关心类型,不需要像tuple那样说明模板, 同模板实例的pair和tuple可以相互转换
			auto ss = std::make_pair((std::string)"sd", (std::string)"shit");
			std::tuple<std::string, std::string> tupless = ss;
			//tuple虽然是一种很方便的传值类型,但是其多样的模板实例使其难以复制赋值
			//tuple 取值 , 使用std::get<元素下表>
			//pair取值 , 同样可用std::get<>,还可用 .first .second
			std::cout << std::get<0>(tupless) << std::endl;
			std::cout << ss.first << std::endl;
		}
		//使用自建结构体(建议是结构体) ,常用,重要
	}

	std::cout << "part34:模板!!" << std::endl;
	{
		//c++的灵魂


		std::cout << "part34:模板!!" << std::endl;
		//templates .在其他语言中有类似概念: 泛型
		//可以将其理解为宏,元编程,函数母版,让编译器根据你的规则编程,一次性实现类似的函数而不受类型限制,不用(手动)重载
		//当我们写了调用函数的源码时,编译器才会根据模板与实际参数类型生成对应函数供调用,不然不存在
		//此为隐式得到实参类型 , 赋给 变量名字 T;
		print("hello,3Q");
		print(911);
		print(11.4514);

		//你可以显式指定参数类型
		print<std::string>("thankyou very much");

		//你可以用来霍霍类,自动生成类
		array<int, 6> ar;
		std::cout<<ar.getSize()<<std::endl;


		//template 可以很nb很nb很复杂很复杂 , 小心使用,适度使用
	}

	std::cout << "part35:栈与堆的比较" << std::endl;
	{
		//栈上只是一句cpu指令
		//堆上调用 malloc 会有一堆指令 , 甚至 cache miss 

		//cherno : 用栈分配内存,当你不能用栈分配时才用堆
		
	}
	


}