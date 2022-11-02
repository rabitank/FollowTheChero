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


		virtual void SetMat4(const std ::string& name , const glm::mat4& mat4 ) = 0;
		virtual void SetMat3(const std::string& name, const glm::mat3& mat3) =0;

		virtual void SetFloat4(const std::string& name, const glm::vec4& vec4) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& vec3) = 0;
		virtual void SetFloat2(const std::string& name, const glm::vec2& vec2) = 0;
		virtual void SetFloat(const std::string& name, float value)			 = 0;

		virtual void SetInt(const std::string& name, int value) =0;
		virtual void SetIntArray(const std::string& name, int* value, uint32_t count) = 0;

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


