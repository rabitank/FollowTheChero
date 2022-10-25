#pragma once
#include <string>
#include <unordered_map>
#include "glm/glm.hpp"

namespace Hazel
{
	class Shader
	{

	public:
		
		virtual ~Shader() {};

		static Ref<Shader> Create(const std::string& filepath);
		static Ref<Shader> Create(const std::string& name, const std::string& vertexsrc, const std::string& fragmentsrc);

		virtual const std::string& Getname() const = 0;

		//Bind,Unbind just for debug mode
		virtual void Bind() const =0;
		virtual void UnBind()const=0;

		//virtual void UploadUniformMat4(const std::string& name, const glm::mat4& mat) = 0;
		//virtual void UploadUniformFloat4(const std::string& name, const glm::vec4& vec) = 0;


	};

	class ShaderLibrary //storage a bunch of shaders
	{
	public:
		void Add(const  Ref<Shader> shader);
		void Add(  const std::string& name, const Ref<Shader> shader);
		
		Ref<Shader> Load(const std::string& name,const std::string& filepath); // assets/texture
		Ref<Shader> Load(const std::string& filepath); // assets/texture

		Ref<Shader> Get(const std::string& name);

		inline bool Exists(const std::string& name) const{ return m_shaderLib.find(name) != m_shaderLib.end(); }

	private:
		std::unordered_map<std::string, Ref<Shader>> m_shaderLib;

	};
}


