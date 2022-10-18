#include "hzpch.h"
#include "OpenGlBuffer.h"

#include "glad/glad.h"

namespace Hazel
{


	/////////////////////////////////////////////
	//////////Vertex buffer /////////////////////
	/////////////////////////////////////////////




	OpenGlVertexBuffer::OpenGlVertexBuffer(float* vertices, uint32_t size) 
	{
		glCreateBuffers(1, &m_rendererID); //相较于Gen,Create方法立即创建并初始化buffer,设置为当前活动buffer,不...它不会
		glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);//设置buffer数据

	}
	OpenGlVertexBuffer::~OpenGlVertexBuffer()
	{

		glDeleteBuffers(1, &m_rendererID);

	}
	void OpenGlVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER,m_rendererID);
	}

	void OpenGlVertexBuffer::UnBind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER,	0);
	}


	/////////////////////////////////////////////
	///////////index buffer /////////////////////
	/////////////////////////////////////////////


	OpenGlIndexBuffer::OpenGlIndexBuffer(uint32_t* indies, uint32_t count)
		:m_count(count)
	{
		glCreateBuffers(1, &m_rendererID); //相较于Gen,Create方法立即创建并初始化buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(uint32_t), indies, GL_STATIC_DRAW);//设置buffer数据

	}

	OpenGlIndexBuffer::~OpenGlIndexBuffer()
	{

		glDeleteBuffers(1,&m_rendererID);
	
	}

	void OpenGlIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
	}

	void OpenGlIndexBuffer::UnBind() const
	{
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}