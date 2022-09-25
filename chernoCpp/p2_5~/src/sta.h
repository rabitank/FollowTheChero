#pragma once
#include <iostream>
//static关键字 

//种类二:类内/结构体中的static关键字
//它会让变量只有一个实例,如果以创建了很多类的实例,但是变量仍然只有一个(指向同一个地址)实例,如果你在一个实例中改变了它,所有的实例中的该变量都会改变
//类似于,类中的全局变量?但是它不再是类的成员,你可以访问它,但不能随着类实例一起初始化
//类中的静态方法同理,它不用实例来调用,而是直接类名::方法名()即可调用,而且方法内部也无法访问到类的实例
struct sta
{
	static int sta_int; //除非const,否则不可初始化
	static int x, y;
	static void print() {//static方法不能使用非static变量,它相当于定义在类外但是需要类名
		//实际上,类实例的方法都会有个隐藏指针this,这是它的工作方式,而静态方法没有this,它不能访问实例成员
		std::cout << x << "," << y << std::endl;
	}

	//比如
	int z, w;
	static void reprint() {
		//std::cout << z << "," << w << std::endl;,报错
	}
};
// int sta::x;
// int sta::y; 由于有两个文件引用的sta.h文件,所以不在这写




