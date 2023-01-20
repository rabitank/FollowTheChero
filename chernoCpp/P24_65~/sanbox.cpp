#include <iostream>

#define LOG(x) std::cout<<x<<std::endl

void PrintName_constRef(const std::string& name)
{};

void PrintName_L_Ref( std::string& name)
{};

void PrintName_R_Ref(std::string&& name)
{};

void PrintName(std::string&& name)
{};

void PrintName(const std::string& name)
{};


class  String
{
public:
	int m_size;
	char* m_data;

	String(const char* str)
	{

		printf("Create\n");
		m_size = strlen(str);
		m_data = new char[m_size];
		memcpy(m_data, str, m_size);
	}

	String(const String& other) 
	{
		printf("Copied\n");
		m_size = other.m_size;
		//m_data = other.m_data; //直接复制指针会导致 double free
		m_data = new char[m_size];
		memcpy(m_data, other.m_data, m_size);
	}

	String(String&& other) noexcept //移动构造函数 , 移动,区别与复制,它不重新为已有数据分配内存,而是移动数据所在内存的所有权
	//noexcept 告诉编译器它不该抛出异常
	{
		printf("Moved\n");
		m_size = other.m_size;
		m_data = other.m_data; //新的左值获得所有权
		other.m_data = nullptr; // 旧的,暂时的右值失去数据内存所有权,之后马上就被销毁.
		other.m_size = 0;
		//这样的显然不适合拷贝构造,拷贝构造参数也是左值,它的存续由使用者说了算 , 不应该被擅自改变.

	}

	String& operator=(const String& other) //拷贝赋值
		//很像拷贝构造,但是还是得写
	{
		printf("l_equal\n");
		m_size = other.m_size;
		m_data = new char[m_size];
		memcpy(m_data, other.m_data, m_size);
		return *this;
	}

	String& operator=(String&& other)
		//与移动构造不同,this已经初始化有数据,接受move进来之前需要丢弃已有数据,不然造成数据泄漏
	{
		printf("r_moveEqual\n");

		if (this == &other) //自移动赋值检测
		{
			return *this;
		}
		delete[] m_data; //回收自己旧数据内存; 

		m_size = other.m_size;
		m_data = other.m_data; //获得所有权

		other.m_data = nullptr; // 旧的,暂时的右值失去数据内存所有权,之后马上就被销毁.
		other.m_size = 0;
		

		
		return *this;
	}

	~String()
	{
		printf("Destoried\n");
		delete[] m_data;
	}

	void Print()
	{
		for (int i = 0; i < m_size; i++)
		{
			printf("%c",m_data[i]);
		}
		printf("\n");
	}



};


struct Entity
{
	String m_string;
	Entity(const String& name)
		:m_string(name)
	{

	}

	Entity(String&& name)
		:m_string(std::move(name) ) //rlReference -> call String move construct function
		//:m_string((String&&) name) //rlReference -> call String move construct function

		//******** 有名字的右值引用是左值, 这点在左值右值说过,c++会折叠引用 ********
		//需要通过类型转换保持右值属性 , 如果没有(String&&) , 还是会使用拷贝构造.
		//或者通过 std::move 保持其左右值属性
	{

	}
	
	void PrintName()
	{
		m_string.Print();
	}

}; 

int main()
{
	LOG("P65: 左值右值 (P85) ");

#if 0
	{
		int i = 10;
		int& j = 10; //& 左值专用引用
		const int& z = 10; // 实际上: int temp =10; const int& z =temp;	
		const int& z_l = i; //同时支持 rl;

	}
	{
		std::string name_1 = "rabit";
		std::string name_2 = "tank";
		PrintName_L_Ref(name_1 + name_2);
		PrintName_constRef(name_1 + name_2);
		PrintName_R_Ref(name_1 + name_2);


		std::string&& name_3 = "Best"; //右值引用能引用右值,但它是左值,有名字有地址
		PrintName_L_Ref(name_3);
		PrintName_constRef(name_3);
		PrintName_R_Ref(name_3);

		//const& 与 && 在不同情况下的优先级
		PrintName(name_1 + name_2);
		PrintName(name_1);


	}
#endif
	LOG("P66: 持续集成 静态分析 (P86,87) "); 
	//CI
	//广告,介绍了Jenkins , 持续集成(CI)能够在代码提交后立即自动构建,运行测试,一般架构在服务器上管理提交代码,审查,减少开发人员的测试工作.
	//静态分析
	//在你编写代码时的分析,检查变量,单词拼写,分析语法错误,给你画波浪线. 广告

	LOG("P67: 参数计算顺序 (P88) ");
	//参数计算顺序,c++标准并未对此进行定义,在参数上进行计算是未定义行为,C++17及以后对此规定先对参数后缀运算需要顺序运算传值,不能并行.

	LOG("P68: 移动语义(P89) ");
#if 0
	{
		Entity entity("rabit tank");
		entity.PrintName();

		std::cin.get();
	}
#endif
	
	LOG("P69: 移动语义_stdmove与移动赋值(P90) ");
	{
		String&& rrstr = "RabitTank_r";
		String lstr = "RabitTank_l";

		String rrstr_t_lstr_no_move("");
		String lstr_t_lstr_no_move("");
		
		String rrstr_t_lstr_move("");
		String lstr_t_lstr_move("");

		LOG("无移动赋值");
		rrstr_t_lstr_no_move = rrstr;
		lstr_t_lstr_no_move = lstr;

		
		LOG("移动赋值");
		rrstr_t_lstr_move = std::move(rrstr); //当没有移动赋值时,l_equal也是可以接受的
		lstr_t_lstr_move = std::move(lstr);
		lstr.Print();

		rrstr_t_lstr_move.Print();
		rrstr_t_lstr_move = std::move(rrstr_t_lstr_move);
		rrstr_t_lstr_move.Print();
		//std::move 并不保持原左右属性,而是都附上右值属性;
		
		
		std::cin.get();
	}


}