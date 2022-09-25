#include <iostream>



class Printobj {
	//因为它只用来搞一个纯虚函数,我们叫它接口(编译器把含有纯虚函数的类称为抽象类)
	//其他的类都需要继承他来获得这一函数的使用权,与引用该类的函数的承认
	//(其他语言有interface,但c++中仍然只是类而已)
public:
	virtual std::string GetClassName()=0;
	//virtual std::string TotalVir() = 0;//声明一个纯虚函数
};

class Entity:public Printobj{
public:
	std::string Getname() {
		return "Entity";
	}
	virtual std::string VirGetname() {
		return "Entity";
	}
	std::string GetClassName()override {
		return "Entity";
	}

};

class Player :public Entity//Entity已经实现了纯虚函数,如果player没有写自己的虚函数就会找继承的
{
private:
	std::string m_Name; // std::string = const char*
public:

	Player(const std::string& name)
		:m_Name(name){}
	std::string Getname() { return m_Name; }
	std::string VirGetname() override { return m_Name; }
	//可以用override标记重写了虚函数,如果这不是一个虚函数则会报错提示你
	//override不加也可以,它只是标记
	std::string GetClassName()override {
		return "Player";
	}
};

void PrintName(Entity* entity) {
	std::cout << entity->Getname() << std::endl;
}
void VirPrintName(Entity* entity) {
	std::cout << entity->VirGetname() << std::endl;
}
void Part(int x, const char* name) {
	std::cout << "<=================="
		<< "Part" << x << ":" << name << "==================>" << std::endl;

}
void PrintClass(Printobj* obj) {
	//只能是指针,保证了进来的参数有GetClassName()这一函数
	//它的意义似乎是,某些函数可以通过要求含有某种函数声明的接口类的指针
	//来获得某个类中函数的使用保证,比如GetClassName
	std::cout << obj->GetClassName() << std::endl;
}


int main() {
	Entity* e = new Entity();
	PrintName(e);
	Player* p = new Player("rabit");
	PrintName(p);
	//由指针类型决定基类子类同名函数的查找方式,先找和指针类型相同的类的方法,然后找子类/基类
	//由于指针类型,在查找方法时,使用属于Entity的方法,形参=(赋值)实参
	//而不是根据实际指向的内容,使用属于player的方法
	//如果我们需要编译器根据指向内容动态选择使用函数(重写),要引入 虚表 ,
	//将基类中的原函数设置为虚函数(使用虚函数有额外开销)
	Part(9, "虚函数");
	VirPrintName(e);
	VirPrintName(p);

	Part(10, "纯虚函数");
	//纯虚函数,没用实现/定义,只由声明,只能由他的子类来实现
	//包含纯虚函数的基类无法实例化(类似虫后???),没实现虚函数的子类也不能
	PrintClass(e);
	PrintClass(p);




}