#include<iostream>
//part7:enum
#include "Player.h"
// const static int A = 0; //ABC未被分组,会对其他变量造成干扰
// const static int B = 1;
// const static int C = 2;
// 
// int main() {
// 	int value = B;//value可以被赋其他值,以至不能判断,这有些危险
// 	if (value==B) {
// 		//do smthing
// 	}
// }

class Log
{
public:
	enum Loglevel//可以在类内定义:类名的命名空间内的枚举类型
	{
		LogLevelError = 0, LogLevelWarning, LogLevelInfo,
		//error
		//类内枚举不能和方法同名,一般会将枚举命名为 类名+意义
	};
private:
	Loglevel m_Loglevel = LogLevelInfo;//m_表示这是一个私有变量,默认任何信息都会被打印
public:
	void setLevel(Loglevel level) {
		m_Loglevel = level;

	}
	void error(const char* mesg) {
		if (m_Loglevel >= LogLevelError)
			std::cout << "[ERROR]:" << mesg << std::endl;
	}
	void warn(const char* mesg) {
		if (m_Loglevel >= LogLevelWarning)
			std::cout << "[WARNING]:" << mesg << std::endl;
	}
	void info(const char* mesg) {
		if (m_Loglevel >= LogLevelInfo)
			std::cout << "[INFO]:" << mesg << std::endl;
	}

};

void function() {
	Entity e(10,1);
	e.print();
}
enum Example:unsigned char //可以指定枚举的数据类型(这里是8位)
{
	A, B, C ,D=6,E
	//0,1,2,6,7 实际值,可以用来判断,可以把他们当整数用
};
int main() 
{
	Log log;
	log.setLevel(Log::LogLevelInfo);//需要加上类名(命名空间)
	log.info("<===========part7:enum===========>");
	Example value = B; //赋值只能是ABC
	if (value==1) {
		//do smthing
	}
	log.info("<===========part7:构造函数===========>");

	log.info("构造函数,每当创建一个实例时就会被调用");
	log.info("记得初始化!");
	Entity e;
	e.print();
	//类实例没有初始化的情况下是随机值(只分配了地址,没有写入内容)
	//,这里print可以运行,但是如果直接访问e.x/y编译不会通过
	e.Init();
	e.print();
	Entity E(3, 4);
	E.print();
	//当你使用静态方法时,构造函数并不会执行,因为你没有创建实例
	//new 也会使用构造函数
	
	//或者直接不能实例了,只能用里面的静态方法
	//Entity ea(3, 4, 5, 6); //报错

	log.info("<===========part7:析构函数===========>");
	log.info("析构函数,每当删除一个实例时就会被调用");
	log.info("一个非静态的变量,其生命周期通常是所在函数从进入到返回的时间");
	log.info("析构函数作用一是,你需要删除初始化的值,以免泄露");
	log.info("如果你初始化的实例成员在堆上,那么你需要在析构里手动删除它");
	log.info("你可以手动调用它,但别这么做,它只是单纯执行一遍函数,在被删除时还会执行一遍");
	//这里为了演示使用funtion
	function();//删除函数的实例 

	log.info("<===========part8:继承===========>");
	log.info("子类是父类的超集,你可以在需要父类的地方使用子类");
	log.info("你可以重写方法代替父类方法");
	Player player;
	player.x=2;
	player.print();
	std::cout << sizeof(Player) << std::endl;


	

}
//这一行后开始删除之前的两个entity实例