#include <iostream>


using String = std::string;
class Entity
{
private:
	std::string m_Name;
	int m_score;

public:
	//成员初始化列表
	//无论你怎么写,它都是按照上面成员的定义顺序来初始化的
	//所以必须按照定义顺序写初始化列表
	//它能让代码易读简洁,还能减少消耗(避免有些变量初始化两遍)
	//一般初始化是,在定义时就进行一次初始化变量,然后在初始化函数中用新的覆盖掉旧的
	//但初始化列表只会初始化一次,这节省了资源
	Entity()
		:m_Name("Unknow"),m_score(0){}

	Entity(const std::string& Name)
		:m_Name(Name),m_score(0){};

	Entity(int score)
		:m_Name("unknow"), m_score(score) {};
	 explicit Entity(const String& Name, int score)
		:m_Name(Name), m_score(score) {};
	const String GetName() const
	{
		return m_Name;
	}

};

void PrintEntity(const Entity& entity) {
	std::cout << entity.GetName() << std:: endl;
}


void main() {
	std::cout << "Part17:成员初始化列表" << std::endl;
	Entity e;
	std::cout << e.GetName() << std::endl;

	std::cout << "Part18:三元运算符" << std::endl;
	// ? = if;
	{
		int a = 1;
		int b = 2;
		if (a > 5)
			b = 10;
		else
			b = 5;
		//等同于

		b = a > 5 ? 10 : 5;//只是为了一个赋值,一行简单明了,速度会快
		b = a > 5 ? a > 10 ? 15 : 10 : 5;
		//等同于a > 5 ? (a > 10 ? 15 : 10) : 5;
		b = a > 5 && a < 100 ? a > 10 ? 15 : 10 : 5;
		//b = (a > 5 && a<100) ?( a > 10 ? 15 : 10) : 5;
		//三元运算符建议不要嵌套,不要直接嵌套连个括号都没 :(

	}
	
	std::cout << "Part19:创建对象的姿势" << std::endl;
	{
		//在函数栈结构中的创建 stack
		Entity entity;//初始化
		Entity entity1("rabit");//初始化-指定参数
		Entity entity2 = Entity("tank");//初始化-一般方法
		//建构一个栈结构的标志 { }, }栈结构结束,在一个函数中,你可以手动主动添加{}来限定一些局部变量的作用域
		Entity* ep;
		{
			Entity entity3;
			ep = &entity3;
		}//entity3被销毁

		//栈一般都很小,1,2mb,当你需要大量的实例时请用堆 heap

		Entity* entity4 = new Entity("傻逼");//new 返回一个对应堆的void指针
		Entity* eph;
		eph = entity4;
		delete entity4;
		//c++中的堆,你需要手动删除堆,并且堆分配更慢
	}

	std::cout << "Part20:new" << std::endl;
	{
		//new 是一种 operater ,你可以重载它
		int* b = (int*)new int[20]; //80bytes in heap ,一整块
		Entity* ep2 = new Entity();
		//new 1. malloc(sizeof(Entity))分配内存 2.调用Entity的构造函数
		//可以指定分配哪一块内存
		Entity* ep3 = new(b) Entity;
		delete ep2;
		delete[] b;
		//delete也是操作符

	}

	std::cout << "Part21:隐式构造函数与类型转换" << std::endl;
	
	{
		//隐式转换只能有一步:entity 含有 std::stirng , int 
		Entity entity = (std::string)"rabit";//const char 显式-> std::string ->entity;一次
		Entity entity1 = 20;//int -> entity 一次
		Entity entity11 = { 20 };
		PrintEntity(22);//int -> entity 一次
		PrintEntity((String)"杀马特团长");//const char 显式-> std::string ->entity;一次

		//explicit 关键字 
		//禁止类的构造函数帮助隐式转换,减少偶然的非正常转化,无意识的转换
		Entity entity3("sb", 20);
		Entity entity4 = { "我他妈来辣",20 };//看报错

	}
}