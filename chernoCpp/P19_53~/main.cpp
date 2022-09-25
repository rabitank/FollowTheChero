#include<iostream>
#include<chrono>
#define LOG(x) std::cout<<x<<std::endl

class entity
{
public:
	virtual void printname(){}	

};
class player :public entity
{


};
class enemy:public entity	 
{

};


class Timer
{
public:
	Timer()
	{
		m_start=std::chrono::high_resolution_clock::now();
	}
	~Timer()
	{
		stop();
	}
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
	void stop()
	{
		auto end = std::chrono::high_resolution_clock::now();
		auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_start).time_since_epoch().count();//microsecond 微秒
		auto stop = std::chrono::time_point_cast<std::chrono::microseconds>(end).time_since_epoch().count();//microsecond 微秒
		auto duration = start - stop;
		auto ms = duration * 0.001;
		LOG(duration << "/us(" << ms << "/ms");

	};
};
int main()
{
	LOG("part53:dynamic_cast动态类型转换");
	{
		//沿继承层次结构进行的强制类型转换
		//类似于函数 , 他在运行时进行转换判断,因此你不会在写代码时得到报错
		//dynamic_cast 的操作数必须包含多态类类型(确定这个类有派生重写),即包含虚函数表()

		//子类转基类 可以,它同时就是基类
		//基类转子类(隐式转换都不能通过) 有可能不行,我们不知道这个基类指针指向内存是不是子类,或者甚至它指向的其实是另一种子类(更糟糕了,访问不存在的成员会导致崩溃 )
		//基于此我们需要dynamic_cast进行(转换)判断,判断指向内存是否是指定类型 如果返回空指针,那么不是
		
	
		entity* actualplayer = new player;
		enemy* ene = (enemy*)actualplayer; //你看,很危险,没报错

		enemy* ene1 = dynamic_cast<enemy*>(actualplayer);
		if (!ene1)
		{
			LOG("actualplayer cant cast to enemy");
		}
		player* pla = dynamic_cast<player*>(actualplayer);
		if (pla)
		{
			LOG("actualplayer can cast to player");
		}
		if (dynamic_cast<player*>(actualplayer)) //最好不要这么用...判断一下没必要改人家类型,可以像上面先赋个值再判断
		{
			LOG("actualplayer is player"); //c#里就是 actualplayer is player直接判断
		}
		//dynamic_cast进行(类型)判断
		
		//dynamic_cast 之所以知道是因为 它使用了运行时的类型信息 rtti 
		//如果你把 rtti在 项目属性 c++ 语言 里关掉 dynamic_cast 就用不了了	
	
	}
	LOG("part54:基准(性能)测试"); {
		//cherno用timer
		{
			Timer timer;
			int i = 0;
			for (int j = 0; j < 10000; j++)
				i += 2;
		}
		//要注意的是,在release模式下,如果你的代码在编译后会被优化的面目全非,这种情况进行时间测试是毫无意义的,因为他们根本没按你写的来
		//但是只有release下的测试才重要,因为你不会在debug下发布项目
	}
}