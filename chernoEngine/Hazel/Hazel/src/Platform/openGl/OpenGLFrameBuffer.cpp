#include "hzpch.h"
#include "OpenGLFrameBuffer.h"
#include "Glad/glad.h"

namespace Hazel
{


	static uint32_t s_maxFrameBufferSize = 8192;

	namespace Utils //Utils:实用工具
	{


		//GL_TEXTURE_2D_MULTISAMPLE same to make miniMap?
		static GLenum TextureTarget(bool multisampled)
		{
			return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void CreateTexture(bool mulitySamples, uint32_t* ids, uint32_t count)
		{

			glCreateTextures(TextureTarget(mulitySamples), count,(GLuint*)ids);

		}

		static bool IsDepthFormat(FrameBufferTextureFormat format)
		{
			switch (format)
			{
			case FrameBufferTextureFormat::DEPTH24STENCIL8:
				return true;
			}
			return false;
		}

		static void BindTexture(bool multisampled, uint32_t id)
		{
			glBindTexture(TextureTarget(multisampled),id);
		};

		//internalFormat:interalStorageFormat 
		//format:访问format
		static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat,GLenum format, uint32_t width, uint32_t height, int index)
		{
			bool multisamples = samples > 1;
			if (multisamples)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);//always GL_RGBA

				//MaybeTemporary 
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}
			//HZ_CORE_TRACE("GL_COLOR_ATTACHMENT0+index{0}", index);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+index, TextureTarget(multisamples), id, 0);

		}
		static void AttachDepthTexture(uint32_t id, int samples, GLenum format,GLenum attachmentType, uint32_t width, uint32_t height)
		{
			bool multisamples = samples > 1;
			if (multisamples)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else
			{
				glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);//always GL_RGBA

				//MaybeTemporary 
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER,attachmentType, TextureTarget(multisamples), id, 0);

		}

		static GLenum HazelFBTextureFormatToGL(FrameBufferTextureFormat format ) 
		{
			switch (format)
			{
			case FrameBufferTextureFormat::RGB:return GL_RGB8;
			case FrameBufferTextureFormat::RGBA8:return GL_RGBA8;
			case FrameBufferTextureFormat::RGBA16F: return GL_RGBA16F;
			case FrameBufferTextureFormat::RED_INTEGER:return GL_RED_INTEGER;
			case FrameBufferTextureFormat::DEPTH24STENCIL8:return GL_DEPTH24_STENCIL8;
			default:break;
			}

			HZ_CORE_ASSERT(false,"ErrorFBTextureFormat");
			return 0;

		}






	}


	
	

	//textureType None also will be Created;
	OpenGlFrameBuffer::OpenGlFrameBuffer(const FrameBufferSpecification& spec)
		:m_specification(spec)
	{
		for (auto& attch : m_specification.Attachments)
		{
			if (!Utils::IsDepthFormat(attch.Format))
			{
				m_colorAttachmentspecifications.emplace_back(attch);
			}
			else
			{
				m_depthAttchmentSpecification = attch;
			}
			
		}
		
		//Currently just for debug!
		Invalidate();
	}

	OpenGlFrameBuffer::~OpenGlFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_rendererID);
		glDeleteTextures((uint32_t)m_colorAttachmentIDs.size(),m_colorAttachmentIDs.data());
		glDeleteTextures(1,&m_depthAttachmentID);
	}

	//you should Bind FrameBufferBefore  : smt wrong
	int8_t OpenGlFrameBuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
	{
		HZ_CORE_ASSERT(attachmentIndex < m_colorAttachmentIDs.size(), "attachmentIndex is out of Index");
		glReadBuffer(GL_COLOR_ATTACHMENT0 +attachmentIndex);
		int pixelData;

		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);//width,height means select a bounds ->and we just read one Pixel
		return pixelData;
	}

////////////
// 
//			一个支持OpenGL渲染的窗口(即帧缓存) 可能包含以下的组合：
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
// 
////////////
	void OpenGlFrameBuffer::Invalidate()
	{


		if (m_rendererID) //第n(n!=1)次创建fb时旧的所有fb/显示图像
		{
			glDeleteFramebuffers(1, &m_rendererID);
			glDeleteTextures((uint32_t)m_colorAttachmentIDs.size(), m_colorAttachmentIDs.data());
			glDeleteTextures(1, &m_depthAttachmentID);

			m_colorAttachmentIDs.clear();
			m_depthAttachmentID = 0;
		}

		glCreateFramebuffers(1, &m_rendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);


		bool multisample = m_specification.Samples > 1;

		if (m_colorAttachmentspecifications.size())
		{
			m_colorAttachmentIDs.resize(m_colorAttachmentspecifications.size());

			Utils::CreateTexture(multisample, m_colorAttachmentIDs.data(),(uint32_t) m_colorAttachmentIDs.size()); 


			for (int i =0;i<m_colorAttachmentIDs.size();i++)
			{
				Utils::BindTexture(multisample, m_colorAttachmentIDs[i]);
				switch (m_colorAttachmentspecifications[i].Format)
				{
				case FrameBufferTextureFormat::RGB:
					Utils::AttachColorTexture(m_colorAttachmentIDs[i], m_specification.Samples, GL_RGB8, GL_RGBA ,m_specification.width, m_specification.height,i);
					break;
				case FrameBufferTextureFormat::RGBA8:
					Utils::AttachColorTexture(m_colorAttachmentIDs[i], m_specification.Samples, GL_RGBA8, GL_RGBA, m_specification.width, m_specification.height, i);
					break;
				case FrameBufferTextureFormat::RGBA16F:
					Utils::AttachColorTexture(m_colorAttachmentIDs[i], m_specification.Samples, GL_RGBA16F, GL_RGBA ,m_specification.width, m_specification.height, i);
					break;
				case FrameBufferTextureFormat::RED_INTEGER:
					Utils::AttachColorTexture(m_colorAttachmentIDs[i], m_specification.Samples, GL_R32I, GL_RED_INTEGER,m_specification.width, m_specification.height, i);
					break;

				}

			}
		}

		if (m_depthAttchmentSpecification.Format != FrameBufferTextureFormat::None)
		{
			Utils::CreateTexture(multisample,&m_depthAttachmentID, 1); 
			Utils::BindTexture(multisample, m_depthAttachmentID);

			switch (m_depthAttchmentSpecification.Format)
			{
			case FrameBufferTextureFormat::DEPTH:
				Utils::AttachDepthTexture(m_depthAttachmentID, m_specification.Samples,GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_specification.width, m_specification.height);
				break;
			}
		}


		if (m_colorAttachmentIDs.size() > 1)
		{ 
			HZ_CORE_ASSERT(m_colorAttachmentIDs.size() < 4, "Max 4 ColorAttachments for oneFrameBuffer");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2,GL_COLOR_ATTACHMENT3 };
			glDrawBuffers((uint32_t)m_colorAttachmentIDs.size(), buffers);
		}
		else if(m_colorAttachmentIDs.empty())
		{
			//Only Depth-pass
			glDrawBuffer(GL_NONE); //
		}


		HZ_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "OpenGL Error:FrameBuffer Invalidate failed!"); //检查frameBuffer是否完成配置
		// GL_INVALID_ENUM error generated. Invalid buffer target enum. 啊?
		glBindBuffer(GL_FRAMEBUFFER, 0);//safe!..还不确定是否使用这个帧


#if 0

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
#endif

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

	void OpenGlFrameBuffer::ClearColorAttachment(uint32_t attachmentIndex, int value)
	{
		HZ_CORE_ASSERT(attachmentIndex<m_colorAttachmentIDs.size(),"ClearColorAttachment'index is out of range")

		auto& spec = m_colorAttachmentspecifications[attachmentIndex];
		glClearTexImage(m_colorAttachmentIDs[attachmentIndex], 0,
			Utils::HazelFBTextureFormatToGL(spec.Format), GL_INT, &value);




		
	}

	void OpenGlFrameBuffer::ReSize(uint32_t width, uint32_t height)
	{
		if(width==0||height==0||width>s_maxFrameBufferSize ||height>s_maxFrameBufferSize)
		{
			HZ_CORE_ERROR("Attempted to resize frameBuffer {0},{1} out of range",width,height);
				return;
		}
		m_specification.width = width;
		m_specification.height = height;
		Invalidate();
	}

}
