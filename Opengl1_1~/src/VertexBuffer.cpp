#include "VertexBuffer.h"


#include "renderer.h"
VertexBuffer::VertexBuffer(const void* Data, unsigned int size)
{
	GLCall(glGenBuffers(1, &m_rendererID));//建立1个 缓冲区 , bufferID建立的缓冲区ID(opengl的大多数东西都有一个无符号整数ID)
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID));//绑定(选择)缓冲区(之后使用这个缓冲区) , (该缓冲区的使用方式 , bufferID)
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, Data, GL_STATIC_DRAW));
	//实际渲染中,顶点不只位置这一个属性,我们需要指定一个顶点包含多少属性,哪些属性,增加一个顶点的数据量s
	//gl需要我们告诉它顶点缓冲区的布局,在它眼里内存里不过是一堆浮点数
	//以及里面的内容,比如坐标,法线,uv等等	
}

VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_rendererID));

}

void VertexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID));
}

void VertexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
