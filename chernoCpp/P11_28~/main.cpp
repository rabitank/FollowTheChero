#include <iostream>
#include <vector>

struct Vertex
{
	float x, y, z;
	Vertex(float ix, float iy, float iz)
		:x(ix), y(iy), z(iz)
	{
	}
	Vertex()
		:x(0), y(0), z(0)
	{
	}

	Vertex(const Vertex& other)
		:x(other.x) , y(other.y) , z(other.z)
	{
		std::cout << "Vector<Vertex> is copying" << std::endl;

	}
	~Vertex()
	{
		std::cout << "delete V" << std::endl;
	}
};

std::ostream& operator<<(std::ostream& stream, const Vertex& vertex)
{
	stream << vertex.x << "," << vertex.y << "," << vertex.z << "," ;
	return stream;
}

int main()
{
	std::cout << "part28:动态数组" << std::endl;
	{
		//讲到标准库模板的 vector ,这玩意就是动态数组 ??? 不用给出具体大小 实际上是删掉旧的,建一个新的
		//标准库的东西虽好,但不是最快的,大多时候为了速度需要自建标准库如EASTl
		std::vector<Vertex> vertics; // 类型 <元素类型> 这是模板 vertics 存储在一块内存中,挨在一起,这样比较块
		//有时用指针作为元素类型,这样不会怼原始数据进行复制,会节省很多资源
		//cherno名言 : 尽量使用对象而不是指针 ,指针是你最后的选择
		vertics.push_back(Vertex(1, 2, 2));  //在末尾添加元素
		vertics.push_back({ 1,1,4 });   //vector 有 size属性

		for (const Vertex& v : vertics) // 哈哈,被秀了, c++的in 应该是 const Vertex& v = vertics[i]
		{
			std::cout << v << std::endl;
		}
		//vertics.clear();//清空vertics
		vertics.erase(vertics.begin() + 1); // 橡皮擦除erase 删掉第二个元素 ,他的参数是 vector<vertex>的const_iterator 一个数组迭代器
		for (int i = 0; i < vertics.size(); i++)
		{
			std::cout << vertics[i] << std::endl;
		}//写函数时避免复制!!!

	}
	std::cout << "part29:动态数组2" << std::endl; //你的第一次优化
	{
		{
		std::vector<Vertex> vertics;//vertics 未定义长度的vector长度为零,任何添加行为都需要删除再新建
		vertics.push_back(Vertex(1, 2, 2));  //vertics 重生+1空位 , Vertex(122)在栈上被创建 然后 被复制到新的空位里 调用拷贝构造函数 ,
		vertics.push_back(Vertex(3, 3, 4));	//vertics 重生+1空位 , vertex(122) 被复制到新vertics , vertex(334)被复制到多的新空位,调用拷贝构造函数*2
		}
		//优化1:直接在vector的分配内存里创建Vertex避免复制
		//优化2:提前设置Vector的容量,在某个数之前不需要重生来+1
		std::cout << "part29:动态数组2_优化后" << std::endl;
		std::vector<Vertex> vertics;
		vertics.reserve(3);//预设元素空位数量 3 , 你不能std::vector<Vertex> vertics(3):这不只是分配内存,还尝试构造了3个vertex
		vertics.emplace_back(1, 2, 2);//传递构造函数的参数列表 , 让 vector自己利用这些参数创建Vertex
		vertics.emplace_back(3, 3, 4);
		vertics.emplace_back(4, 4, 5);

	}




}