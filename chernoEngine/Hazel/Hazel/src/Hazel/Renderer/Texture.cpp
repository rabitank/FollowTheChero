#include "hzpch.h"
#include "Texture.h"

#include "Platform/openGl/OpenGlTexture.h"
#include "Renderer.h"

namespace Hazel
{

	Ref<Texture2D> Texture2D::Create(const std::string& filePath)
	{
		switch (Renderer::GetAPI())
			{//make_shared 会调用构造函数,括号中为构造参数,这要求能够实例化,不可以使用抽象类    ->所以这跟texture显示有个鸡毛关系啊你妹!
			case RendererAPI::API::OpenGl: return CreateRef<OpenGlTexture2D>(filePath); //不是 public 继承方式甚至不允许转换到基类...是因为这样可以访问到基类吗?
			case RendererAPI::API::None: HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			}
			HZ_CORE_ASSERT(false, "Unkonw RendererAPI,Texture::Create filed!")
				return nullptr;

	}

	

	Hazel::Ref<Hazel::Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{//make_shared 会调用构造函数,括号中为构造参数,这要求能够实例化,不可以使用抽象类    ->所以这跟texture显示有个鸡毛关系啊你妹!
		case RendererAPI::API::OpenGl: return CreateRef<OpenGlTexture2D>(width,height); //不是 public 继承方式甚至不允许转换到基类...是因为这样可以访问到基类吗?
		case RendererAPI::API::None: HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		}
		HZ_CORE_ASSERT(false, "Unkonw RendererAPI,Texture::Create filed!")
			return nullptr;
	}

}
