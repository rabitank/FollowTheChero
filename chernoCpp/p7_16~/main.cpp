#include<iostream>

class Entity
{
private:
	std::string m_Name;
	mutable int m_DebugCount = 0;
public:
	const std::string& GetName() const
	//后const的作用原理,是在作用域中,给用到的成员变量加一个const
	{	
		m_DebugCount++;
		//mutable不会实际改变变量类型,这点和const不同,
		//但他会让变量逃过const方法给变量赋const的流程
		//可以说是针对类的const方法的关键字
		return m_Name;
	}
protected:

};



void main() {
	//16 mutable 关键字
	const Entity e;
	e.GetName();

	//用到mutable的另一个地方,lambda,类似一次性的小函数,可以赋给变量
	int x = 0;
	auto f = [=]() mutable{
		x++;//mutable: int x = 0;x++;
		std::cout << x<<" lambda hello!" << std::endl;
	};
	//[&] 捕获,捕获符& 对局部变量进行引用捕获得到 &x , &e
	//[=] 捕获,捕获符 = ,对局部变量进行按值传递得到x,e的值,是一些常量
	//对于=的只读限定,可以在lambda假声mutable,使得可以修改
	//(实际是创建了同名的局部变量并对应赋值),并不会影响外面的变量
	f();
	//你平时不会在lambda用到mutable的
	
	std::cout << x << std::endl;

}