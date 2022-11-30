#include "hzpch.h"
#include "OpenGlUniformBuffer.h"
#include "glad/glad.h"

namespace Hazel
{
	//OpenglUniform在创建时就在gpu初始化并绑定了layout全局可用了
	OpenGlUniformBuffer::OpenGlUniformBuffer(uint32_t size, uint32_t binding)
	{
		glCreateBuffers(1, &m_rendererID);

		//创建并初始化缓冲区对象的数据
		glNamedBufferData(m_rendererID, size, nullptr, GL_DYNAMIC_DRAW); // TODO: investigate usage hint

		glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_rendererID);
		//bingding : index in GPU -> ayout(std140, binding = 0) uniform Camera 

	}

	OpenGlUniformBuffer::~OpenGlUniformBuffer()
	{
		glDeleteBuffers(1, &m_rendererID);
	}

	void OpenGlUniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
	{
		//更新缓冲区对象数据存储的子集
		//offset:指定缓冲区对象数据存储的偏移量 数据替换将从何处开始，以字节为单位。
		//其他懂得都懂
		glNamedBufferSubData(m_rendererID,offset,size,data);

	}

}