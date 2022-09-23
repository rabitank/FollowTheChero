#include "IndexBuffer.h"
#include "renderer.h"

//记得,有红点的文件没有被包括在项目中,不会被编译,要手动添加
IndexBuffer::IndexBuffer(const unsigned int* Data, unsigned int count)
	:m_count(count)
{
	ASSERT(sizeof(unsigned int) == sizeof(GLuint));
	GLCall(glGenBuffers(1, &m_rendererID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID));//绑定(选择)缓冲区(之后使用这个缓冲区) , (该缓冲区的使用方式(插槽) , bufferID)
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(unsigned int), Data, GL_STATIC_DRAW));
		
}

IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1,&m_rendererID));
}

void IndexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID));
}

void IndexBuffer::Unbind() const
{

	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0));
}
