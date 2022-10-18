#include "Texture.h"
#include "vendor/stb_image/stb_image.h"//联想总是从工程文件目录sln开始

Texture::Texture(const std::string& filepath)
	:m_rendererID(0),m_filePath(filepath),m_localBuffer(nullptr),m_height(0),m_weight(0),m_BPP(0)//order
{
	stbi_set_flip_vertically_on_load(1);//垂直翻转纹理,为了让纹理像素从左下角开始来满足gl要求
	//取决于你的纹理格式
	m_localBuffer = stbi_load(filepath.c_str(), &m_weight, &m_height, &m_BPP, 4);
	GLCall(glGenTextures(1, &m_rendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D,m_rendererID));//指定存储类型配置工具 对容器进行进行配置
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));//Parameteri:参数 MIN_FILTER:缩小过滤器 GL_Linear:线性
	//设置2d纹理的参数:缩小方式(过滤器)为 线性
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));//Parameteri:参数 MAG_FILTER:放大过滤器 GL_Linear:线性
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));//Parameteri:参数 WRAP_S:水平环绕,CLAMP:嵌入
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));//Parameteri:参数 WRAP_T:垂直环绕,CLAMP:嵌入
	//将组好的tex2D设置以及其他参数发送给gl,显卡利用这些信息创建空间 
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,m_weight,m_height,0,GL_RGBA,GL_UNSIGNED_BYTE,m_localBuffer));//Parameteri:参数 WRAP_T:垂直环绕,CLAMP:嵌入
	//深度:0,
	//内部格式(gl_读取纹理的方式):GL_RGBA8(8bit/通道) ,
	//宽,
	//高,
	//边框像素宽度,
	//格式(纹理数据的实际格式),
	//通道数据的类型:8bit/通道->unsigned char :gl以及可以根据以上参数分配空间
	//纹理数据的指针(data会被copy到显卡) 可以暂时是空

	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	if (m_localBuffer)
		stbi_image_free(m_localBuffer);//释放



}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_rendererID));
}

void Texture::Bind(unsigned int slot /*=0 */) const
{
	GLCall(glActiveTexture(GL_TEXTURE0+slot)); //使用纹理插槽1
	GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererID));
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
