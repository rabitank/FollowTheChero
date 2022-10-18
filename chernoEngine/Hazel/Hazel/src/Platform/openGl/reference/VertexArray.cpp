#include "VertexArray.h"

#include "renderer.h"
#include "BufferLayout.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_rendererID));
	GLCall(glBindVertexArray(m_rendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_rendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& buffer, const BufferLayout& bl)
{
	Bind();
	buffer.Bind();

	const  auto elements = bl.GetElements();
	unsigned int offset = 0;
	for(unsigned int i =0; i<elements.size();i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.ifNormalized,bl.GetStride(), (const void*) offset));
		//实际上是链接了当前绑定的顶点缓冲区,这实际上创建了/赋值了一个顶点数组
		//(指定这个属性(在多个属性中)的索引 , ->对应于shader中的 layout
		//属性所用的组件(数据)个数(只有1,2,3,4可选),
		//组件的数据类型
		//是否归一化(bool),
		//每个顶点之间的字节偏移量(顶点大小), //实际意义上 类型,组件数量 两个参数
		//属性起始位到该属性的字节偏移量(是一个指针0.0,你可以直接(const void*) num(0不用))(偏移量可以用宏,好改) )		
		offset += element.count * LayoutElement::GetSizeofType(element.type);
	}
	
}

void VertexArray::Bind()const
{
	GLCall(glBindVertexArray(m_rendererID));
}

void VertexArray::Unbind()const
{
	GLCall(glBindVertexArray(0));
}
