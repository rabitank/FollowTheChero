#include "hzpch.h"
#include "Shader.h"


#include "Renderer.h"

#include "Platform/openGl/OpenGlShader.h"

namespace Hazel
{

	Ref<Shader> Shader::Create( const std::string& filepath) //now we read shader form xxsl(xxshader language) , even finally use a all-purpose language
	{
		//which api
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGl:
			return std::make_shared<OpenGlShader>(filepath); //不是 public 继承方式甚至不允许转换到基类...是因为这样可以访问到基类吗?
		case RendererAPI::API::None:
			HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		}
		HZ_CORE_ASSERT(false, "Unkonw RendererAPI ,Shader::Create filed!")
			return nullptr;

	}

	Ref<Shader> Shader::Create(const std::string& name , const std::string& vertexsrc, const std::string& fragmentsrc) //now we read shader form xxsl(xxshader language) , even finally use a all-purpose language
	{
		//which api
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGl:
			return  std::make_shared<OpenGlShader>(name,vertexsrc,fragmentsrc); //不是 public 继承方式甚至不允许转换到基类...是因为这样可以访问到基类吗?
		case RendererAPI::API::None:
			HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		}
		HZ_CORE_ASSERT(false, "Unkonw RendererAPI ,Shader::Create filed!")
			return nullptr;

	}

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader> shader)
	{
		HZ_CORE_ASSERT(m_shaderLib.find(name) == m_shaderLib.end(), "shader already exist");
		m_shaderLib[name] = shader;

	}


	void ShaderLibrary::Add(const Ref<Shader> shader)
	{
		auto& name = shader->Getname();
		HZ_CORE_ASSERT(m_shaderLib.find(name) == m_shaderLib.end(), "shader already exist");
		m_shaderLib[name] = shader;
	
	}

	Hazel::Ref<Hazel::Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		Ref<Shader> shader = Shader::Create(filepath);
		Add(name,shader);
		return shader;

	}

	Hazel::Ref<Hazel::Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		Ref<Shader> shader =  Shader::Create(filepath);
		Add(shader);
		return shader;

	}

	Hazel::Ref<Hazel::Shader> ShaderLibrary::Get(const std::string& name)
	{
		HZ_CORE_ASSERT(m_shaderLib.find(name) != m_shaderLib.end(), "shader don't exist")
		return m_shaderLib[name];
	}

}