#pragma once
class Player
{
public:
	int x, y;
	int speed;
	void Move(int x, int y);

};
//类的作用之一,收集零散变量,方便重复定义类似变量组合
//类内的函数称为,方法
//类内变量称为,成员
//struct 与 class 的唯一区别就是 前者默认是public,而后者默认是private;
//the cherno 的使用习惯是,struct用来构建一些只有变量的数据类型,当然也可以有一些函数,然而它只是一些
//为了方便而建造的抽象的概念数据结构,class则更加复杂,更像一个实际的obj,拥有更多复杂且实际的方法与成员
//另一个区别是,如果需要继承,你最好从上到下都用class来继承,两者可以混合继承,但是编译器会警告你
