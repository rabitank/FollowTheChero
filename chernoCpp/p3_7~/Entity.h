#pragma once
#include <iostream>
class Entity
{
private:
	Entity(int vx,int vy,int vz,int vw) 
	//你甚至可以把构造函数藏到private里,这样你就不能以某种方法直接实例了
	{
		x = vx;
		y = vy;
		z = vz;
		w = vw;
		std::cout << "but you can't!" << std::endl;
	}
public:
	float x, y;//float 4 bites
	float z, w;
	~Entity() {//析构函数
		std::cout << "entity is being deleting num:" <<x<< std::endl;

	}
	Entity()//构造函数需与类同名 ,默认存在的构造函数就是无参的
	{
		z = 0.f;
		w = 0.f;
	}
	//可以定义多个构造函数以进行重载
	Entity(int vx, int vy) 
	{
		x = vx;
		y = vy;
	}
	Entity(int vx, int vy, int vz) = delete;//直接删了这个构造,当然你不写他也不存在


	void print() {
		std::cout << x << "," << y << std::endl;
		std::cout << z << "," << w << std::endl;
		 

	}
	void Init() {
		x = 0; y = 0;
	}

};

