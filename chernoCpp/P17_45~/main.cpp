#include<iostream>
#define LOG(x) std::cout<<x<<std::endl;
#include<algorithm> //for sort
#include<vector>
#include<functional>

struct ver2
{ //写类型,如果类型里啥也不写,也至少会有一个字节
private: int x, y;//这里是8字节
public:
	ver2(int ix, int iy)
		:x(ix), y(iy) {}
	int* getPostion()
	{
		return &x;
	}
};
struct vector2 
{
	float x, y;
};
struct vector4
{
	union 
	{
		struct 
		{
			//匿名struct把这四个数封成一个成员,再把它放到union里
			float x, y, z,w; //看作两个verctor2

			//如果是匿名的,它只是一种数据结构,你只要 .x 就能访问x

		};
		struct 
		{
			float r, b, g, a;
		};
		struct
		{
			vector2 A, B; //A 代表x,y,B代表z,w 
		};

	};
// 
// 	vector2& getA()
// 	{
// 		//得到前两位
// 		return *(vector2*)&x;
// 	}
	
};


class base 
{
public:
	base()
	{
		LOG("base creator!");
	}
	virtual ~base()
	{
		LOG("base destructor!");
	}
	

};
class deriver:public base
{
public: 
//由于派生类 在被创建(删除)时需要调用基类的构造函数(完成基类成员的初始化)/析构函数,
//因此要求其构造函数与析构函数是能被派生类调用的,即public或protected
	deriver()
	{
		LOG("deriver creator!")
	}
	~deriver()
	{
		LOG("deriver destructor!")
	}
};
class anotherclass:public base
{
private:
	int a;
public:
	anotherclass(double ia)
		:a(ia)
	{
		
	}
	~anotherclass()
	{

	}

};
int main()
{
	LOG("part45:排序")
	{
		std::vector<int> vws = { 2,3,4,23,17,932,12 };

		//sort 参数: 1,开始迭代器 2,结束迭代器 3,排序函数/谓词/可选
		//谓词介绍
		//< :升序 默认
		//> :降序
		//std::greater<int>() 一个std的>比较函数模板

		//排序函数:返回类型 bool 如果 你想要a在b前 ,那么返回true
		//排序函数容易出错,你最好充分考虑可能的情况,让每个元素的位置都能被确定下来
		std::sort(vws.begin(), vws.end(), [](int a, int b) {
			if (a == 23)
				return true;
			if (b == 23)
				return false; //强行把23排到第一个
			return a<b;
			});
		for (int value : vws)
			LOG(value);
		
	}
	LOG("part46:类型双关")
	{
		//绕过c++的强类型系统?
		//当你创建某类型变量时就是分配了内存
		//那么你能不能绕过类型限制把内存用来放其他东西呢?


		int a = 114513;
		double b = *(double*)&a; //a的指针转换为double类型指针,以double方式访问地址再赋给b;
		//double& b = *(double*)&a; b =0.0 这样是不行的,你不能在4字节空间写8字节,至少在debug中不行
		LOG(b); //毫无疑问这样的转换是乱的
		int c[2] = {1,0};
		double& d = *(double*)&c;
		d = 1; //这样可以,你分配了8字节可写
		LOG(d);

		ver2  ver = { 2,2 };//你可以直接把ver看成 int[2],他的8字节内存形式适合这么搞
		
		int* ver2a =(int*)& ver;
		LOG(ver2a[0]<<ver2a[1]); //实锤了,[n]就是跳转到一个指针地址加n个变量偏移再解引用,真是随便的操作符呢

		int charz = *(int*)((char*)&ver + 4); //以char方式访问地址,并移动4个char得到地址,再以int访问得到的地址
		LOG(charz);

		//再比如直接得到地址,你可以绕过很多东西直接连接到内存并对其操作
		int* postion = ver.getPostion();
		*postion = 4;
		LOG(ver2a[0] << ver2a[1]);
		//比如写字节流,不要管里面是什么类型,直接把它当字节输出
	}
	LOG("part47:union")
	{
		//c++union like to class
		//你可以像class一样用它,但是它只占用自己一个成员内存(所有成员共用一个成员内存,处理方法跟类型有关),但是名字不同
		//和类型双关有关,再自建类型中有用,也可以减少类型转换和复制
		//它可以有构造函数等 , 但是不能用虚函数

		//匿名union 不能拥有成员函数

// 		struct Union  
// 		{
// 			union 
// 			{
// 				int x;
// 				float y;
// 
// 			};	
// 		};
// 		Union u;
// 		u.x = 2;
// 		LOG(u.x << "," << u.y);
		//
		//
		vector4 ver4 = { 1.0,2.0,3.0,4.0 };
		LOG(ver4.A.x << ver4.A.y);
		ver4.y = 500.0;
		LOG(ver4.A.x << ver4.A.y);

	}
	LOG("part48:虚析构函数")
	{
		base* ins1 = new base();
		delete ins1;
		LOG("-------------------------------");
		deriver* ins2 = new deriver;
		delete ins2;
		LOG("-------------------------------");
		base* in3 = new deriver;
		delete in3;
		//只有base的析构函数被调用了,但是deriver没有,这导致deriver部分成员没有被删除造成内存泄漏
		//c++不知道可能下面会有另一个析构函数,
		//这就是为什么需要 virtual 标记为virtual，意味着这个类可能会被重写,c++就会知道在层次结构下的这个方法可能被重写了。,还有析构函数需要调用
		//

		//虚析构方法和一般的虚方法的不同是
		//虚方法 是等待被复写,复写后只调用复写后的方法
		//虚析构方法则会和派生析构方法一起被调用 ,先调用派生析构方法后虚析构方法 
		LOG("-------------------------------");

		//deriver* in4 = new base;
		

	}
	LOG("part49:c++风格的类型转换")
	{
		//casting 被用来指代类型转换 ,你理解一下
		
		//c风格的
		double a = 3.2;
		int	b = (int)a + 5.6; //去尾cast
		LOG(b)
		//c++风格的
		//他们和c比起来并没有做多余的事情,只是细分了c中转换类型并显式注明,编译器会在编译时检查这样的(这种类型的)转换能否成功
		//但是c++的类型转换帮助我们理解代码,减少错误,便于搜索
			// 四种cast static_cast reinterpret_cast dynamic_cast const_cast
			//都是语法糖而已,c都能做 static 静态转换 reinterpret 双关(改变指针类型,重解释内存,而不改写内存内容) const (绕过)/附加上const
		
			//static_cast:编译器隐式执行的任何类型转换都可以由static_cast来完成,一次转换
		int s = static_cast<int>(a) + 5.6;
		LOG(s)
			//double ca = reinterpret_cast<base*>(&a)+3.3; //static_cast<base*>(&a);无效,此处应为一个双关
			//dynamic_cast:派生类之间转换,并且动态检查?
		base* devi = new deriver;
		anotherclass* ac = dynamic_cast<anotherclass*>(devi);
		//这里ac是空的,devi指向的是deriver,dynamic_cast在运行时发现了这样转换不符合类关系,将ac赋为空
		anotherclass* ac2 = static_cast<anotherclass*>(devi);
		//ac2不为空 static_cast就只会傻乎乎地把指针赋给ac2
		deriver* dc = dynamic_cast<deriver*>(devi);
		//dc存在,dynamic_cast允许转换

		//cherno: 我知道c++的转换很好,但是我无法割舍c转换

		std::cin.get();


	}
}