#include <iostream>
#include <memory> //smart ptr

class Entity
{
public:
	Entity()
	{
		std::cout << "Created Entity" << std::endl;
	};
	~Entity()
	{
		std::cout << "Destroy Entity" << std::endl;
	};
	int a=1;
};

class String
{
private:
	char* m_Buffer;
	unsigned int m_size;
public:
	String(const char* string)
	{
		m_size = strlen(string);//strlen不计\0终止符, strcopy会一起复制\0
		m_Buffer = new char[m_size+1];
		memcpy(m_Buffer, string, m_size);
		//复制内存 ,const void* 中 void代表任何数据类型 
		//复制string的前m_size位
		m_Buffer[m_size] = 0;
	}	
	friend std::ostream& operator<<(std::ostream& stream, const String& string);
	//friend 允许外部函数访问内部private成员
	~String() {
		delete m_Buffer;
	}
	char& operator[]( const unsigned int& index ) {
		return m_Buffer[index];//没错,[]的返回就是char&
	}
	String(const String& other)//拷贝函数的签名形式
		:m_size(other.m_size)
	{
		std::cout << "copy happened" << std::endl;
		//memcp(this,&other,sizeof(String)),默认拷贝构造函数,你还可以写成m_Buffer(),m_size(),
		//删除拷贝函数,不允许拷贝 , String(const String& other) = delete;
		m_Buffer = new char[m_size + 1];
		memcpy(m_Buffer, other.m_Buffer, m_size + 1 ); // 这里用sizeof(other.m_buffer)会只写入4字节,因为当成指针了,用sizeof(*other.m_Buffer)也行
	}

};

class smartPtr
{
	Entity* m_obj;
public:
	smartPtr(Entity* obj)
		:m_obj(obj)
	{

	}
	~smartPtr() {
		delete m_obj;
	}
	Entity* operator->() {
		return m_obj;

	}//为了能让smartptr 指针和正常指针一样使用->,进行重构

};

struct Vector3
{
	float x, y, z;
};
std::ostream& operator<<(std::ostream& stream, const String& string)
{
	stream << string.m_Buffer;
	return stream;
}

void PrintString(const String& string)
{
	std::cout << string << std::endl;
}

int main() {

	std::cout << "part25:smart Ptr" << std::endl;
	//多用用smartptr QAQ,自动管理内存
	//unique ptr 作用域自动销毁,你不能复制uniqueptr,他和内存是绑定的
	//它的开销很低
	{
		std::unique_ptr<Entity> entity(new Entity()); 
		//不能用= new Entity() ,这需要隐式调用构造函数,而unique_ptr的构造是explicit
		std::unique_ptr<Entity> e1 = std::make_unique<Entity>();
		//另一种初始化方法,更安全,最好的
		//std::unique_ptr <Entity> e2 = e1;
		//不行,uniqueptr没有拷贝构造函数,不允许复制,不允许分享
	}
	//shared_ptr 分享指针,weak_ptr 弱指针
	{
		std::shared_ptr<Entity> e1 = std::make_unique<Entity>();
		//shared_ptr指针需要分享!当它被复制时引用计数加1,创建时算上自己引用计数是1;
		//当引用计数到0时释放内存
		std::shared_ptr<Entity> e2(new Entity());
		//不要这样初始化,这将把分配new Entitty 和 引用计数的控制块 分为两步分配,降低效率

		//指针接力
		std::shared_ptr<Entity> e3;
		{
			{
				std::shared_ptr<Entity> e4 = std::make_shared<Entity>();
				e3 = e4; //该内存引用:2
			}//e4 godie,e3 alive 内存引用:1 !=0 ,内存存活
			
		}
	//weakptr 弱指针, 不会增加sharedptr的引用,摆烂指针,活着就活着,死了就算了
		//你可以询问weak 当前内存是否存活/过期
		std::weak_ptr<Entity> e4 = e3;
		//内存引用仍为1;

	}

	std::cout << "part26:复制与拷贝构造函数" << std::endl;
	//避免不必要的拷贝
	// = 赋值操作符,总是在*复制值*
	{
		String string("They are SB"); 
		String second = string; 
		//程序崩溃!! //前提是把写的拷贝构造函数注释掉0.0
		//char* m_Buffer 被赋给了seconde m_Buffer ,他们代表同一块内存
		//delete m_Buffer 意味着释放指向内存
		//这两个String被销毁执行析构函数时,m_Buffer指向的同一块内存要被释放两次
		//第二次释放时,编译器认为它已经被清空,不能再次被代码控制清空
		//这引起了崩溃
		//而这就是所说的,浅拷贝,只复制了指针
		second[9] = 'N';
		
		PrintString(string); PrintString(second);
		//所以我们需要重写-拷贝构造函数 , 它是在赋值时调用的一种构造函数,使得其使用深拷贝方式
	}
	std::cout << "part27:箭头操作符->" << std::endl;
	{
		//说过了 
		Entity e;
		Entity* ptre = &e;
		ptre->a; //等于 (*ptre). 解引用加访问成员

		smartPtr entity = new Entity;
		entity->a;//你看

		//为了smartptr 重构 ->操作符
		//使用->得到偏移量 ,炫技

		int py = (int)&((Vector3 *) 0)->z; //获取从0地址开始偏移的x的地址并写成int ,得到偏移量
		std::cout << py << std::endl;

	}
}
//cherno 名言 :总是使用const引用 去传递独享
//到处复制只会拖慢你