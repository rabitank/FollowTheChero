#include <iostream>
//#include <string>
//yes,you need it to print std::string by std::cout
//but now you don't

void PrintString(const std::string& string) {
	//string += "h";//it is const ,you can't
	std::cout << string << std::endl;
}
//c++的参数传递就是一个等号 
//类型 形参 = 实参;如果不是指针或引用,大多数时候是在拷贝实参
//这会占用内存:(

//const
class Entity
{
private:
	 int m_X, m_Y;
	 mutable int var;
public:
	 int GetX() const 
	 { 
		//const在方法名之后,!!只有类中可以这么用!!
		//意为,不会修改任何实际的类,(不会动成员变量),只读类的数据
		
		 //如果你非要改变某个变量在const中,你可以使用 mutable 关键字
		 //意为可变的,改变它不会引起const冲突
		 var = 4;

		 return m_X;//供const类实例调用
	 }
// 	 int GetX() { //供非const类实例调用
// 		 return m_X;
// 	 }
	 //所以有时候一种方法会有两个版本
	 int GetY() //现在我们不能保证GetY不会修改类
	 { 
		
		 return m_Y;

	 }

private:

};

void PrintEntity(const Entity& e,Entity& f) {//引用被初始化后不允许修改指向,只能修改它自己
	//加上const之后 即const Entity ,修改e 即修改一个const Entity,这同样不行
	std::cout << e.GetX() << std::endl;
	//普通实例能够使用const方法,但是const实例不能使用普通方法
	std::cout << f.GetX() << std::endl;
	//std::cout << e.GetY() << std::endl;
	//const Entity类型与自己的成员 GetY不兼容
	//GetY并未保证自己不会修改成员,这与const Entity保证不变冲突
	//即,间接修改也不行
}


int main() {
	std::cout << "<==========Part13:string==========>" << std::endl;
	const char* name = "Rabit";
	//"Rabit"存储在内存的const只读部分,即是能够不加const使用char*,你仍然难以改变它
	//name[2]='c';不要这么做,这没用,甚至会出错
	char namearray[] = "Rabit";
	namearray[2] = 'c';//数组是可以的

	char name2[5] = { 'R','a','b','i','t' };
	char name3[6] = { 'R','a','b','i','t' };//"Rabit"多加一个\0终止符,占6字节
	//const char* 是c风格的字符串
	//const 在C11以前并不必要,但是一般不会改动字符串所以会加上
	//c11后, 字符串格式固定为 const char*(和编译器有关)
	//字符串内存固定,无法扩展
	//"Rabit"并未使用new ,仍在栈上,不需要也不能用delete[]
	std::cout << name << std::endl;
	std::cout << name2 << std::endl;//烫烫烫是内存守卫cc
	//std::cout << name3 << std::endl;
	//name尾跟一个00 字节0,表示字符串终止
	//没有这个字符终止符,就只是一个字符数组

	std::string stringname = "Rabit";//左边是string,右边是const char*
	std::cout << stringname << std::endl;
	stringname.size();

	//std有类为string,在c++中你应该使用它,他是baiscstring模板的一个char实例
	//包含了char数组和内置函数
	//如果是const char*或char*,你就得使用c函数来处理他们,如strlen(),strcpy()
	//+在string中被重载了,你可以
	stringname += " tank!";//这样衔接string与const char*
	//find方法->contain
	bool contain = stringname.find("an") != std::string::npos;
	//npos 非法位置,虚无位置,意味不存在
	
	//字符串传参
	PrintString("sb");


	std::cout << "<==========Part13:stragestring==========>" << std::endl;
	//中断
	const char name4[11] = u8"Rabit\0tank";
	//u8没什么意思,和普通字符串一样-utf8
	std::cout << strlen(name4) << std::endl;
	
	//宽字符 L""
	const wchar_t* namewchar = L"Rabit";
	//widechar 有的编译器(vs) 决定它为 双字节

	//char16,32
	const char16_t* name16 = u"Rabit"; //双字节?-utf16
	const char32_t* name32 = U"Rabit";//四字节?即utf32

	using namespace std::string_literals;//一些函数/语法糖

	std::string namestring = "Rabit"s + "tank";
	//s : 返回一个std::string
	//std::wstring L""  std::u32string U""  std::u16.....
	const char* nameR = R"(Rabit
Tank
is 
pretty
)"; //让生活更美好
	//R"()" 忽略转移字符,随便打/\和回车,会忠诚地输出他们
	const char* nameRex = "Rabit\n"
		"tank\n"
		"is wonderful\n";
	std::cout << nameR << nameRex << std::endl;

	//字符字面量
	"SB"; //这就是一个字符字面量,只存储在只读内存中
	const char namesurfacecopy[] = "SB";//这其实是复制"SB"到栈上地数组


	std::cout << "<==========Part14:const==========>" << std::endl;
	//const 并不会实际影响代码编译,他只是面对开发人员地机制,一种承诺
	//承诺不会改动
	const int MAX_AGE = 90;//似乎写到了只读区
	const int* a = new int;//你不能修改指针指向的内容,只能读取
	//还可以写成 int const* a
	//注意,并非是指针不变,而是指向的内容不能变
	//*a = 2;
	a = &MAX_AGE;//如果是 int* a,强制将&MAX_AGE从constint*变为int*才能执行这句
	//如果之后强行赋值可能会让程序崩溃
	std::cout << *a << std::endl;

	//2
	int* const b = new int;
	//可以修改指向内容,但是不能修改指针
	*b = 2;
	//b = (int*)&MAX_AGE;

	//3
	const int* const c = new int;
	//不能修改内容 const* 不能修改指针 *const






}
