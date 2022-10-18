#pragma once


#include <vector>
#include "renderer.h"


struct LayoutElement
{
	unsigned int type;//GL里是的
	unsigned int count;
	unsigned char ifNormalized;

	static unsigned int GetSizeofType(unsigned int type) {
		switch (type)
		{
		case GL_FLOAT:return sizeof(GLfloat);
		case GL_UNSIGNED_INT:return sizeof(GLuint);
		case GL_UNSIGNED_BYTE:return sizeof(GLbyte);

		};
		ASSERT(false);
		return 0;

	}

};


class BufferLayout
{

private:
	std::vector<LayoutElement> m_Elements;
	unsigned int m_Stride;
public:
	BufferLayout()
		:m_Stride(0) {}; //vector 提供 index
	template<typename T> //模板特化前的写的通用逻辑
	void Push(unsigned int count) //模板特化,参数记得保持一致
	{
		static_assert(false,"此乃静态断言字符串,你必须指定模板");

	};
	template<>//模板特化(指定<>类型的函数模板),必须指定<>
	void Push<float>(unsigned int count) //<type1,type2 >(type1 x,type2 y) 模板指定类型与参数对应(话说复习下模板啊喂!
	{
		m_Elements.push_back({ GL_FLOAT,count,GL_FALSE });
		m_Stride += sizeof(float)*count;
	};
	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({GL_UNSIGNED_INT,count,GL_FALSE });
		m_Stride += sizeof(int) * count;
	};
	template<>
	void Push<unsigned char>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE,count,GL_TRUE});
		m_Stride += sizeof(unsigned char) * count;
	};
	inline std::vector<LayoutElement> GetElements() const { return m_Elements; }
	inline unsigned int GetStride() const{ return m_Stride; }


};

