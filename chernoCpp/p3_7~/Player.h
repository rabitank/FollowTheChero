#pragma once
#include "Entity.h"//继承需要当前文件有类的声明


//Player同时是Player与Entity两种类型
class Player:public Entity
{
	//Entity中所有不是private的成员都能在player实例中被调用
public:
	const char* name; //char point 4bites
	void Printname() {
		std::cout << name << std:: endl;
	}

};

