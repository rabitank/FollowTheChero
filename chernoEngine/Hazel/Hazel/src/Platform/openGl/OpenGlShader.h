#pragma once
#include "Hazel/Renderer/Shader.h"
namespace Hazel
{

	class OpenGlShader:public Shader
	{
	public:
		OpenGlShader(const std::string& filepath);
		~OpenGlShader();


		//Bind,Unbind just for debug mode
		virtual void Bind() const ;
		virtual void UnBind()const;

	private:
		unsigned int m_rendererID;
		std::string m_Filepath;
		mutable std::unordered_map<std::string, int> m_UniformLocationCache;
		std::tuple<std::string, std::string> parseShader(const std::string& filepath);
	};
}


