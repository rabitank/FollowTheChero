#pragma once
#include "Hazel/Renderer/Shader.h"

typedef unsigned int GLenum;

namespace Hazel
{

	class OpenGlShader:public Shader
	{
	public:
		OpenGlShader(const std::string& filepath);
		OpenGlShader(const std ::string& name , const std::string& vertexsrc, const std::string& fragmentsrc);

		~OpenGlShader();

		inline virtual const std::string& Getname() const { return m_name; }

		//Bind,Unbind just for debug mode
		virtual void Bind() const ;
		virtual void UnBind()const;
		
		
		
		void UploadUniformInt(const std::string& name,  int mat) ; //悲报, 提交uniform被本地化了....以后得动态投射了
		
		void UploadUniformFloat(const std::string& name,  float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& vec);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& vec);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& vec);

		void UploadUniformMat3(const std::string& name, const glm::mat3& mat);
		void UploadUniformMat4(const std::string& name, const glm::mat4& mat);
		

	private:
		unsigned int m_rendererID;
		std::string m_Filepath;

		std::string m_name;
		mutable std::unordered_map<std::string, int> m_UniformLocationCache;//we will active it soon....
		std::string readFile(const std::string& filepath);
		std::unordered_map<GLenum,std::string> preProcess(const std::string& source);
		void shaderCompile(const std::unordered_map<GLenum, std::string>& shaderSource);
	};
}


