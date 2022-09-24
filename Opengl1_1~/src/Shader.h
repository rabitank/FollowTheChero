#pragma once
#include <iostream>
#include<unordered_map>//for hashmap
#include "glm/glm.hpp"
#include <unordered_map>


struct shaderPSource
{

	std::string vertexshader;
	std::string fragmentshader;
	
};

class Shader
{
private:
	unsigned int m_renderer;
	std::string m_Filepath;
	mutable std::unordered_map<std::string, int> m_UniformLocationCache;


public:
	Shader(const std::string& sourceCode);
	~Shader();
	void Bind() const;
	void Unbind() const;

	void SetUniform4f(const std::string name,float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const std::string name,const glm::mat4& mat);
	void SetUniform1f(const std::string name,float v0);
	void SetUniform1i(const std::string name,int i0);
private:
	shaderPSource parseShader(const std::string& filepath);
	int GetUniformLocation(const std::string& name) const;
	unsigned int CompileShader(const std::string& source, unsigned int type);
	unsigned int createshader(const std::string& vertexshader, const std::string& fragmentshader);



};

 