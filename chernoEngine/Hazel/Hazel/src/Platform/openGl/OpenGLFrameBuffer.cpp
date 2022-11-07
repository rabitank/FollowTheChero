#include "hzpch.h"
#include "OpenGLFrameBuffer.h"
#include "Glad/glad.h"

namespace Hazel
{

	OpenGlFrameBuffer::OpenGlFrameBuffer(const FrameBufferSpecification& spec)
		:m_specification(spec)
	{
		
		
		//Currently just for debug!
		Invalidate();
	}


	OpenGlFrameBuffer::~OpenGlFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_rendererID);
		glDeleteTextures(1, &m_colorAttachMent);
		glDeleteTextures(1, &m_depthAttachMent);
	}

////////////一个支持OpenGL渲染的窗口(即帧缓存) 可能包含以下的组合：
// 
// 			· 至多4个颜色缓存
// 
// 			· 一个深度缓存
// 
// 			· 一个模板缓存
// 
// 			· 一个积累缓存
// 
// 			· 一个多重采样缓存
//////////////////
	void OpenGlFrameBuffer::Invalidate()
	{

		if (m_rendererID) //第一次创建fb时删除所有fb/显示图像
		{
			glDeleteFramebuffers(1, &m_rendererID);
			glDeleteTextures(1, &m_colorAttachMent);
			glDeleteTextures(1, &m_depthAttachMent);
		}

		glCreateFramebuffers(1, &m_rendererID);
		glBindFramebuffer(GL_FRAMEBUFFER,m_rendererID);

		glCreateTextures(GL_TEXTURE_2D,1,&m_colorAttachMent);
		glBindTexture(GL_TEXTURE_2D,m_colorAttachMent);//safe!
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_specification.width, m_specification.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorAttachMent,0);
		//附加贴图至帧缓冲...提供某个buffer的值
		//比如提供颜色: GL_COLOR_ATTACHMENT0 提供深度:GL_DEPTH_ATTACHMENT 作为模板(蒙版):GL_STENCIL_ATTACHMENT。(附着位只有三个..? 同时作为深度与模板 GL_DEPTH_STENCIL_ATTACHMENT
		

		glCreateTextures(GL_TEXTURE_2D, 1, &m_depthAttachMent);
		glBindTexture(GL_TEXTURE_2D, m_depthAttachMent);//safe!
		//glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_specification.width, m_specification.height);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_specification.width, m_specification.height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);//GL_DEPTH24_STENCIL8 深度模板格式:融合深度和纹理，可以让你设置深度缓冲区的同时设置模板缓冲区。
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthAttachMent,0);

		HZ_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER)== GL_FRAMEBUFFER_COMPLETE,"OpenGL Error:FrameBuffer Invalidate failed!"); //检查frameBuffer是否完成配置


		glBindBuffer(GL_FRAMEBUFFER, 0);//safe!..还不确定是否使用这个帧缓存

	}

	void OpenGlFrameBuffer::Bind() const
	{	
		glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);
		glViewport(0, 0, m_specification.width, m_specification.height); //设置视口
	}

	void OpenGlFrameBuffer::UnBind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0	);

	}

	void OpenGlFrameBuffer::ReSize(uint32_t width, uint32_t height)
	{
		m_specification.width = width;
		m_specification.height = height;
		Invalidate();
	}

}
