#pragma once


template<typename T, size_t S>
class Array
{
public:
	constexpr size_t Size() { return S; } //并没有储存S,编译后S被替换为一个实际的常量 , 它返回一个常量表达式,可以作为常量使用
	//这一切建立在模板在编译时就被确定的基础上

	T& operator[](int i)  //返回左值 , 可修改模式
	{
		if (!(i < S))
		{
			__debugbreak(); //msvc的debug模式中断
		}
		return m_data[i];
	}

	const T& operator[](int i) const //只读 , const类型的array引用也会用const版的
	{
		return m_data[i];
	}

	T* Data() { return m_data; }
	const T* Data() const { return m_data; }

private:
	T m_data[S]; //模板在使用时,S必须是常量
};