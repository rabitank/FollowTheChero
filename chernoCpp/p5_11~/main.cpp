#include <iostream>
#include <array>

class Entity {
	int X, Y;//默认private
	//private:只有Entity内的成员/和friend标记可以访问它,读取更改它
public:
	Entity() {
		X = 0;
	}
protected:
	int Z;
	//protected:该类内与派生类内可以访问它
public:
	int W;
	//所有人可见
};

class Player :public Entity
{
public:
	Player() {
		//X = 2;//不行,entity外
		Z = 2;//可以,protected
	}
};

void Part(int x, const char* name) {
	std::cout << "<=================="
		<< "Part" << x << ":" << name << "==================>" << std::endl;

}

///////////////////////////

void main() {
	Part(11, "访问修饰符");//也叫可见修饰符
	Entity e;
	//e.X = 2;不行,entity之外
	//e.Z = 2;不行,protected
	//可见性和代码运行无关,他只是为了人们交流代码,帮助自己而产生的
	//public 是用来使用的,对外的,
	//你不应该直接调用private方法和更改private变量,他们是隐藏的内部逻辑
	//如果非正常使用private,甚至会破坏类的功能

	Part(12, "Array");

	int example[5];			//在栈上,在跳出作用域时销毁
	int* another = new int[5];//在堆上,手动销毁或程序结束
	for (int i = 0; i < 5; i++)
		example[i] = 2;
	for (int i = 0; i < 5; i++)
		another[i] = 2;
	delete[] another;//如果你想返回在函数中新创建的数组,就要用new
	//类中的new 会导致内存间接寻址(先访问this)

	example[0] = 2;
	example[4] = 4;
	
	//example[-1] = 5;
	//example[5] = 10;
	//这在release下是允许的,但你改变了不属于自己的内存(即指针前4字节与指针后20~24字节)
	
	std::cout << example[0] << std::endl;
	std::cout << example << std::endl;//打印数组地址
	//访问元素/写入元素 访问地址:指针+下表*类型字节数 
	//如example[2] = example + 2*4;
	
	int* ptr = example;
	*(ptr + 2) = 6;
	//等于 example[2]=6;等于 *(int*)((char*)ptr+8)=6
	//+2被重载为 ptr + 2*ptr类型字节数

	//std中有重写的array(std::array),含有边界检测,读取大小等方法,这些事原生没有的
	//手动求原生数组的count并不靠谱,你需要自己维护(储存其长度)
	int a[5];
	int counta = sizeof(a) / sizeof(int);//5
	int* b = new int[5];
	int countb = sizeof(b) / sizeof(int);//1 指针的长度为4字节
	const int examplesize = 5;//在类中还要加上static才能用
	int exampleconst[examplesize];
	std::array<int, 5> stdarray;//一个stdarray,有额外开销
	std::cout << stdarray.size() << std::endl;
}