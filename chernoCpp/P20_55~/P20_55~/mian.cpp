#include<iostream>
#define LOG(x) std::cout<<x<<std::endl

#include <tuple>
#include <string>

#include <optional>
#include <fstream>

#include <variant>

#include <any>

std::tuple<std::string, int> CreatePerson() //两个变量 pair也行 tuple支持更多类型与参数
{
	return { "smart tuanzhang",20 }; //不用构造体了

}

//optional c++17  特性,指针属性为主
std::optional<std::string> ReadFileAsString(const std::string& filePath)
{

	std::ifstream streamR(filePath);
	if (streamR)
	{
		std::string str;
/*		outSuccess = true;*/
		//read
		std::getline(streamR, str);
		streamR.close();
		return str;
	}

/*	outSuccess = false;*/
	return {};

}

//varraint c++17

enum class Error
{
	None = 0 ,TypeError,UnknowError
};

std::variant<std::string,int> ReadFileAsStringByVarriant(const std::string& filePath) //varriant 加个错误码
{

	std::ifstream streamR(filePath);

	if (streamR)
	{
		std::variant<std::string,int> str;
		/*		outSuccess = true;*/
				//read
		
		streamR.close();
		return str;
	}

	/*	outSuccess = false;*/
	return {};

}

int main()
{
	LOG("Part55:结构化绑定"); //tuple
	{
		auto  me = CreatePerson();
		std::cout << std::get<0>(me) << std::endl; //是的 , 访问成员有点烦人


		int age;
		std::string name;
		std::tie(name, age) = CreatePerson(); //std::tie 提供的引用访问方法,依然有点麻烦

		//c++17 结构化绑定 ,把标准设置一下
		auto [m_name, m_age] = CreatePerson();
		std::cout << m_name << ":" << m_age << "\n"; //不再有只用一次的结构类型了!!!

 	}
	LOG("Part56:OPTIONAL数据处理- 视频已到P76"); //数据状态未知状况下的读取
	{
		bool ifsuccesss = false;
		std::optional<std::string> content = ReadFileAsString("data.txt");
// 		if (content != "")
// 		{
// 
// 		}
// 		if (ifsuccesss)
// 		{
// 
// 		}

		std::string value = content.value_or("can.t open it! "); //如果有 返回值 ,没有/读取失败 返回括号内内容

		if (content ) //optional 自带bool,赋值了会有true content.has_value()
		{
			std::cout << *content << std::endl;
			content.value(); //访问value 如果有
			LOG("you did it");
		}
		else
		{
			std::string& str = *content;
			std::cout << "you can't do " <<value<< std::endl;
			

		}
	}
	//c++17 兼容性变量
	LOG("Part57:单一变量存放多类型数据	");
	{
		std::variant<std::string, int> data;
		data = "rabitank";
		std::string str = std::get<std::string>(data);
		//int inta = std::get<int>(data); cant
		LOG(str);
		data = 2;
		data.index(); //1 返回当前data内容的类型的类型索引  <type1,type2,type3,type4>  0是type1,1是....
		std::string* strptr = std::get_if<std::string>(&data); //如果指针指向类型是string 就get 返回指向内容的指针 反之 nullptr
		if (strptr)
		{
			LOG(*strptr);
		}
		//建议用 get_if()  , 安全
		//variant 还是类似类 内存是成员之和
		int a  = std::get<int>(data);
		LOG(a);
		data = "qiumi!";
		//data = false;//可以,但是赋了未定义类型的值后就不能访问了


	}
	LOG("Part58:存放任意类型数据_any");
	{
		std::any data; //no need template

		data = "love";
		data = 2;
		data = std::string("i love sword");
		
		// get 
		std::string str = std::any_cast<std::string>(data);//if it is not the type of data,there will be a error

		//if you ver is more than 32u,any will dynamic distribute memory,it is bad
		// but varriant always static distribute the memory

		//cherno: useless
	}
}