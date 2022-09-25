#include <iostream>

using String = std::string;



struct Vector2
{
	float x, y;
	Vector2(const float& ix, const float& iy)
		:x(ix), y(iy) {}
	//for math  ,reload
	Vector2 operator+ (const Vector2& other) const //操作符就是函数,
	{
		return Vector2(x + other.x, y + other.y);
	}
	Vector2 operator* (const Vector2& other) const //你可以分开他们,写一个multiple的函数然后在重载中调用它
	{
		return Vector2(x * other.x, y * other.y);
	}
	bool operator== (const Vector2& other) const
	{
		return x == other.x && y == other.y;
	}
	bool operator!= (const Vector2& other) const
	{
		return !(*this == other);
	}
};

std::ostream& operator<<(std::ostream& stream,const Vector2& vec ) //和vector2关系不大,且关键在std内
{
	stream << vec.x << "," << vec.y;
	return stream; //引用出引用进,只是借用了一段时间:)

}

//在实例内调用外部函数,并需要实例成员时也需要this
//在外面实现一个用到类并且会在类内使用的函数,
//一,函数的定义必须在class定义之后,不然参数不完整,
//二,函数声明必须在class之前,不然类内无法使用,
//三,函数声明用到类,需要在声明前声明类

class Entity;//艹!类声明
void PrintEntity(const Entity& e);
class Entity {
public:
	int x, y;
	Entity(int x,int y ) // 一旦你定义了一个构造,就不再有默认构造函数了
	{
		//Entity* const & e = this; //this 是 Entity* const 这里没写出来,不允许改变this指向,所以你要引用的话也得const,保证不会改写this指针  多说一点,&引用初始化后是不允许再引用其他变量的,相当于已经是 *const(牢记只是另一个名字)
		// Entity* e = this; 最好这么写,简单些,虽然多了一个指针 
		//当然也可以不写,直接用this->x
		//e->x = x;//-> 相当于 *p. 解引用加访问成员
		//e->y = y;

		this->x = x;//因为同名,没有显式访问成员x将无法赋值
		this->y = y;//写完指针后按 . 会自动跳出 ->
		PrintEntity(*this);
		//this,在堆上 ,不要delete this, 实例会爆炸
	}

	int GetX()const
	{
		const Entity* e = this; //这里同样需要加上const ,防止我们能够改变这个实例,这里的this 也被函数的const改成了 const Entity* const类型
		return e->x;

	}


	~Entity() {
		std::cout << "哼哼哼啊啊啊啊啊啊" <<x<<y<< std::endl;
	}

};
void PrintEntity(const Entity& e) {
	std::cout<<e.x<<","<<e.y<<std::endl;
}
int* CreateArray() {
	int Carray[50];
	return Carray; //指针确实传递了,但指向的地址被释放了
	//解决方法是,1,在heap上创建 2,在外部创建,使用函数对变量进行操作
}

class scopeptr
{
private:
	Entity* m_ptr;
public:
	scopeptr(Entity* ptr)
		:m_ptr(ptr) {}
	~scopeptr()
	{
		delete m_ptr;//删除指向内容
	}
};

int main() {
	std::cout << "Part22:操作符,重载" << std::endl;
	{	
	//少用,但有用,小心使用,通常在写一些比较底层的类型和包时使用
	Vector2 position = { 330,345 };
	Vector2 speed = { 2,2 };
	Vector2 position1 = Vector2(3, 4) * speed + position;
	//返回一个vector2 ,但它并不是变量,而是一个常量,因此不能被引用
	// << 左移操作符重载
	std::cout << position1 << std::endl;
	bool test = true;
	std::cout << (position1 == position) << std::endl;//不打括号会成为ostream& == Vector2
	} 
	//↑栈帧

	std::cout << "Part23:this" << std::endl;
	{
		//this是一个指向实例本身的实例隐藏指针成员,我们可以使用它来调用(必须由一个有效对象调用的函数)
		//见class Entity
	}
	std::cout << "Part24:stack" << std::endl;
	//作用域: {} if{} class{}.... "{}"就代表一个作用域,他内存上是栈里的一个栈帧(结构),进入一个作用域就是在push一个栈帧(弹出),push完就丢掉里面的栈内容
	{
		Entity e0(1,2);
		Entity* e1 = new Entity(3, 4);
		scopeptr e3 = new Entity(11, 45);
	}
	//std::cout<<e1<<std::endl //e1,是栈上的指针,同样会被删除,但是e1指向的堆内存不会
	//在实现函数时切记这一点
	//作用域指针,经过类的包装能自动根据作用域delete堆上的对象; scopeptr



}