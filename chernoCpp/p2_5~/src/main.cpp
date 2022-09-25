#include "Player.h"
#include "Log.h"
#include "sta.h"


int si_sta = 10;//与static int si_sta同名的全局变量
extern int exi_sta;//与 sta.cpp中 int exi_sta同名的全局变量,加上extern并去除初始化后,编译器会找到exi_sta,并使这个 exi_sta成为sta.cpp中的exi_sta的引用
//这被称为 external linking,但如果找不到同名的非extern全局变量,则会报错
//函数实例
void function_sta();//链接通过

int x = 10;


//使用例 静态类实例
class Singleton {
public:
	static Singleton& Get() {
		static Singleton instance;//将使用限制于Get()后,当然你可以使用指针来实现,不过会更加麻烦
		return instance;//这样你可以用类实例的成员与方法
	}
	void hello() {
		std::cout << "Singleton:hello" << std::endl;
	}
};
void main() {
	Log log;
	log.info("<==================part5:class==================>");
	log.setLevel(log.LogLevelWarning); //先写的main内代码,确定功能,由此反推log类该怎么实现
	log.warn("hello"); //报错中显示,,字符串作为实参类型为const char* 这就是字符串本质吗??
	log.error("hello");
	log.info("hello");
	Player player;

	log.setLevel(log.LogLevelInfo);
	log.info("<==================part6:static==================>");
	log.info("si_sta:"); std::cout << si_sta << std::endl;
	log.info("static本质上是将一个变量设为静态(生命周期变长,地址恒定)并根据所处位置设定作用域");
	log.info("基本上,static 用的越多越好,因为如果存在大量的同名变量与函数(同声明)在全局中,非常容易起冲突,或者不知不觉被改变了值");
	log.info("你可以将它放在头文件中,这样每个引用它的cpp都会有一个类似于private的自己的变量或函数");
	//sta st = { 2,3 }; //报错了
	sta st;
	st.x = 3;
	st.y = 5;
	st.print();//都可以
	sta::x = 4;//不过最好使用这种方法
	st.print();

	log.info("<==================part6:static-作用域与生命周期==================>");
	Singleton::Get().hello();
	//part7~见p7

}

//种类三:函数中的static变量 函数局部静态变量
void function_sta() {
	static int i = 0; //被限定了作用域在function_sta内
	//第一次被使用初始化,但不会被删除,其生命周期是整个函数
	int j = 1;
	i++;
	std::cout << "j: " << j << "i: " << i << std::endl;
}




