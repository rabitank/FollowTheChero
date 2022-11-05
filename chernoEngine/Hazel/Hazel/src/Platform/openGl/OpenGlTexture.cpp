#include "hzpch.h"
#include "OpenGlTexture.h"

#include <stb_image.h>
namespace Hazel
{

	void OpenGlTexture2D::Bind(uint32_t slot) const
	{
		HZ_PROFILE_FUNCTION();
		//glBindTextureUnit or glBindTexture ,the first will check the texture is a black block
		GLCall( glBindTextureUnit(slot, m_rendererID));//target : 插槽 ,可以一次应在一个槽绑定多个纹理(纹理序列)?


	}




	void OpenGlTexture2D::SetData(void* data, uint32_t size)
	{
		HZ_PROFILE_FUNCTION();
		uint32_t bpp = m_internalFormat == GL_RGBA8 ? 4 : 3;

		//size是我们选择渲染的图像部分buffer大小....一般都是全显示了不会用到它
		HZ_CORE_ASSERT(size == m_width * m_heigth * bpp,"SetData error: texture didn't set entire texture");

		glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_heigth, m_dataFormat, GL_UNSIGNED_BYTE, data);

	}

	OpenGlTexture2D::OpenGlTexture2D(const std::string& path)
		: m_filePath(path)
	{
		HZ_PROFILE_FUNCTION();

		stbi_set_flip_vertically_on_load(1);//垂直翻转纹理,为了让纹理像素从左下角开始来满足gl要求
		stbi_uc* image=nullptr;
		{
			HZ_PROFILE_SCOPE("OpenGlTexture::OpenGlTexture(stbi_load image)");

			//取决于你的纹理格式
			 //the final Position: channel= 0 means don't change the oranginal image channel ,
			image = stbi_load(path.c_str(), &m_width, &m_heigth, &m_BPP, 0);//未来我们会导出 width,height,bpp的
		}
		HZ_CORE_ASSERT(image, "Texture load error:Faild to load image!");


// 		//将组好的tex2D设置以及其他参数发送给gl,显卡利用这些信息创建空间 
// 		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, m_width, m_heigth, 0, GL_RGB, GL_UNSIGNED_BYTE, image));//Parameteri:参数 WRAP_T:垂直环绕,CLAMP:嵌入
 		GLCall( glCreateTextures(GL_TEXTURE_2D,1, &m_rendererID));

		GLenum internalformat = 0,dataformat =0;
		if (m_BPP ==4) 
		{
			internalformat = GL_RGBA8;
			dataformat= GL_RGBA;
		}
		else if (m_BPP==3)
		{
			internalformat = GL_RGB8;
			dataformat = GL_RGB;

			
		}

		m_dataFormat = dataformat;
		m_internalFormat = internalformat;

		HZ_CORE_ASSERT(internalformat & dataformat,"Load Texture error: not Sported image type");

	
		GLCall(glTextureStorage2D(m_rendererID, 1, internalformat, m_width, m_heigth)); // levels:texture纹理层级数(mipmap级别,对于2D纹理必须为1) internalformat 图像 存储格式 //该API指定纹理存储格式
//glTextureParameteri or glTexParameteri , it use id but not enum and don't need bind to set texture
		glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//Parameteri:纹理 MIN_FILTER:缩小过滤器 GL_Linear:线性
		glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//Parameteri:纹理 MAG_FILTER:放大过滤器 GL_Linear:线性
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));//Parameteri:参数 WRAP_S:水平环绕,CLAMP:嵌入
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));//Parameteri:参数 WRAP_T:垂直环绕,CLAMP:嵌入

		//glTextureSubImage2D or glTexImage2D
		GLCall(glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_heigth,dataformat, GL_UNSIGNED_BYTE, image));
		//submit 提交纹理,level:0(原始图像,不使用降采样的mipmap)
		//xoffset pixel position value on x label move (+) xoffset
		// similar to xoffset
 
	
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width
		//, m_heigth
		//, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer);
		//将组好的tex2D设置以及其他参数发送给gl,显卡利用这些信息创建空间 
		//深度:0,
		//内部格式(gl_读取纹理的方式):GL_RGBA8(8bit/通道) ,
		//宽,
		//高,
		//边框像素宽度,
		//格式(纹理数据的实际格式),
		//通道数据的类型:8bit/通道->unsigned char :gl以及可以根据以上参数分配空间
		//纹理数据的指针(data会被copy到显卡) 可以暂时是空


		stbi_image_free(image);


	}

	OpenGlTexture2D::OpenGlTexture2D(uint32_t width, uint32_t height)
		:m_width(width),m_heigth(height),m_BPP(4),m_filePath("")
	{
		HZ_PROFILE_FUNCTION();

		GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID));

		m_internalFormat = GL_RGBA8;
		m_dataFormat = GL_RGBA;
		GLCall(glTextureStorage2D(m_rendererID, 1, m_internalFormat, m_width, m_heigth)); // levels:texture纹理层级数(mipmap级别,对于2D纹理必须为1) internalformat 图像 存储格式 //该API指定纹理存储格式
		glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//Parameteri:纹理 MIN_FILTER:缩小过滤器 GL_Linear:线性
		glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//Parameteri:纹理 MAG_FILTER:放大过滤器 GL_Linear:线性
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));//Parameteri:参数 WRAP_S:水平环绕,CLAMP:嵌入
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));//Parameteri:参数 WRAP_T:垂直环绕,CLAMP:嵌入


	}

	OpenGlTexture2D::~OpenGlTexture2D()
	{
		HZ_PROFILE_FUNCTION();
		glDeleteTextures(1,&m_rendererID);
	}

}