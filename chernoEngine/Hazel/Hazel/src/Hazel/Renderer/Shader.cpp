#include "hzpch.h"
#include "Shader.h"


#include "Renderer.h"

#include "Platform/openGl/OpenGlShader.h"

namespace Hazel
{

	Shader* Shader::Create(const std::string& filepath)
	{
		//which api
		switch (Renderer::s_rendererAPI)
		{
		case RendererAPI::OpenGl:
			return new OpenGlShader(filepath); //不是 public 继承方式甚至不允许转换到基类...是因为这样可以访问到基类吗?
		case RendererAPI::None:
			HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		}
		HZ_CORE_ASSERT(false, "Unkonw RendererAPI ,Shader::Create filed!")
			return nullptr;

	}


}