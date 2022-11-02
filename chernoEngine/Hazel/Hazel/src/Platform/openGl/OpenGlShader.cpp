#include "hzpch.h"
#include "OpenGlShader.h"


#include <sstream>
#include <fstream>
#include <string> //for pareShader

#include "Glad/glad.h"
#include <glm/gtc/type_ptr.hpp>


namespace Hazel
{


	OpenGlShader::OpenGlShader(const std::string& Filepath)
		
	{
		HZ_PROFILE_FUNCTION();
		std::string content = readFile(Filepath);

		auto shadersSource = preProcess(content);

		shaderCompile(shadersSource);



		//  shader/shader.glsl / or shader\shader.glsl
		auto lastSlash = Filepath.find_last_of("/\\"); //find_last_of 允许部分匹配
		// shader.glsl
		lastSlash = (lastSlash == std::string::npos) ? 0 : lastSlash+1;

		auto lastDot = Filepath.rfind('.');
		auto count = lastDot == std::string::npos ? Filepath.size() : lastDot - lastSlash;
		m_name = Filepath.substr(lastSlash, count);


	}
	OpenGlShader::OpenGlShader(const std::string& name , const std::string& vertexsrc, const std::string& fragmentsrc)
	{
		HZ_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> shaderSource;
		shaderSource[GL_VERTEX_SHADER] = vertexsrc;
		shaderSource[GL_FRAGMENT_SHADER] = fragmentsrc;

		shaderCompile(shaderSource);

		m_name = name;

	};

	OpenGlShader::~OpenGlShader()
	{
		HZ_PROFILE_FUNCTION();

		glDeleteProgram(m_rendererID);
	}

	void OpenGlShader::shaderCompile(const std::unordered_map<GLenum, std::string>& shadersSource)
	{

		HZ_PROFILE_FUNCTION();


		//std::vector<GLint> shaderVarry; //shaderVarry(size)实际是设置了size个空占位,push_back从空占位后开始
		//shaderVarry.reserve(shadersSource.size());//设置预设尺寸,push_back仍从0开始,...cherno觉得vector在堆上还是影响性能=_=
		HZ_CORE_ASSERT(shadersSource.size() < 3,"too many kinds component shader ");
		std::array<int, 2> shaderVarry;
		uint32_t index=0;

		uint32_t program = glCreateProgram();
		for (auto& i : shadersSource)
		{
			GLint rendererID = glCreateShader(i.first);
			const GLchar* shaderSrc = i.second.c_str();//& 取地址这个用法,必须是左值 指必须是被存储的变量,这样才有自己的地址,右边只是返回的一个值而已,不会被维护
			glShaderSource(rendererID, 1, &shaderSrc, 0);
			glCompileShader(rendererID);

			HZ_CORE_TRACE("shader type:{0} content:{1} ", i.first, i.second.c_str());


			GLint isCompiled;

			glGetShaderiv(rendererID, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(rendererID, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(rendererID, maxLength, &maxLength, &infoLog[0]);

				HZ_CORE_ERROR("OpenGl Log:{0}", infoLog.data());
				HZ_CORE_ASSERT(false, "OpenGl Error: compilate failed!", i.second);

				glDeleteShader(rendererID);
				shaderVarry[index] =(-1); // I 'm smart!!
				continue;

			}

			glAttachShader(program, rendererID);
			shaderVarry[index++]=rendererID;
			
		
		}



		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			
			for (auto& shader : shaderVarry)
			{
				glDeleteShader(shader);
			}

			m_rendererID = -1;

			HZ_CORE_ERROR("OpenGl Log:{0}", infoLog.data());
			HZ_CORE_ASSERT(false, "OpenGl Error: Program link shader failed!");
			return;
		}

		glValidateProgram(program);

		for (auto& shader:shaderVarry)
		{
			glDetachShader(program, shader);
		}

		m_rendererID = program; //在一切正确之前就拿 program当挡箭牌	

	};


	void OpenGlShader::Bind() const
	{
		HZ_PROFILE_FUNCTION();

		GLCall(glUseProgram(m_rendererID));

	};
	void OpenGlShader::UnBind() const
	{
		HZ_PROFILE_FUNCTION();

		glUseProgram(0); // T_T	

	}

	void OpenGlShader::SetMat4(const std::string& name, const glm::mat4& mat4)
	{
		HZ_PROFILE_FUNCTION();


		UploadUniformMat4(name, mat4);
	}

	void OpenGlShader::SetMat3(const std::string& name, const glm::mat3& mat3)
	{
		HZ_PROFILE_FUNCTION();

		UploadUniformMat3(name, mat3);
	}

	void OpenGlShader::SetFloat4(const std::string& name, const glm::vec4& vec4)
	{
		HZ_PROFILE_FUNCTION();

		UploadUniformFloat4(name, vec4);

	}

	void OpenGlShader::SetFloat3(const std::string& name, const glm::vec3& vec3)
	{
		HZ_PROFILE_FUNCTION();

		UploadUniformFloat3(name, vec3);

	}

	void OpenGlShader::SetFloat2(const std::string& name, const glm::vec2& vec2)
	{
		HZ_PROFILE_FUNCTION();

		UploadUniformFloat2(name, vec2);

	}

	void OpenGlShader::SetFloat(const std::string& name, float value)
	{
		HZ_PROFILE_FUNCTION();

		UploadUniformFloat(name, value);

	}

	void OpenGlShader::SetInt(const std::string& name, int value)
	{
		HZ_PROFILE_FUNCTION();
		UploadUniformInt(name, value);
	}

	void OpenGlShader::SetIntArray(const std::string& name, int* value, uint32_t count)
	{
		HZ_PROFILE_FUNCTION();

		UploadUniformIntArray(name, value, count);
	}

	void OpenGlShader::UploadUniformInt(const std::string& name, int value)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		if (location == -1) HZ_CORE_WARN("uniform {0} in shader {1},didn't find",name,m_rendererID);
		GLCall(glUniform1i(location,(GLint)value)); //transPos 是否转置

	}

	void OpenGlShader::UploadUniformIntArray(const std::string& name, int* value, uint32_t count)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		if (location == -1) HZ_CORE_WARN("uniform {0} in shader {1},didn't find", name, m_rendererID);
		GLCall(glUniform1iv(location, count,value)); //transPos 是否转置
	}

	void OpenGlShader::UploadUniformFloat(const std::string& name, float value)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		if (location == -1) HZ_CORE_WARN("uniform {0} in shader {1},didn't find", name, m_rendererID);
		GLCall(glUniform1f(location, (GLfloat)value)); //transPos 是否转置
	}
	void OpenGlShader::UploadUniformFloat2(const std::string& name, const glm::vec2& vec)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		if (location == -1) HZ_CORE_WARN("uniform {0} in shader {1},didn't find", name, m_rendererID);
		GLCall(glUniform2fv(location,1,glm::value_ptr(vec))); //transPos 是否转置
	}
	void OpenGlShader::UploadUniformFloat3(const std::string& name, const glm::vec3& vec)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		if (location == -1) HZ_CORE_WARN("uniform {0} in shader {1},didn't find", name, m_rendererID);
		GLCall(glUniform3fv(location, 1, glm::value_ptr(vec)));

	}
	void OpenGlShader::UploadUniformFloat4(const std::string& name, const glm::vec4& vec)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		if (location == -1) HZ_CORE_WARN("uniform {0} in shader {1},didn't find", name, m_rendererID);
		GLCall(glUniform4f(location, vec.x, vec.y, vec.z, vec.w));//等价(性能也许不同) 4fv(location,1,float*)); 

	}
	void OpenGlShader::UploadUniformMat3(const std::string& name, const glm::mat3& mat)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		if (location == -1) HZ_CORE_WARN("uniform {0} in shader {1},didn't find", name, m_rendererID);
		GLCall(glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(mat))); //transPos 是否转置
	}
	void OpenGlShader::UploadUniformMat4(const std::string& name, const glm::mat4& mat)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		if (location == -1) HZ_CORE_WARN("uniform {0} in shader {1},didn't find", name, m_rendererID);
		GLCall( glUniformMatrix4fv(location, 1, GL_FALSE,glm::value_ptr(mat))); //transPos 是否转置
	}


	std::string OpenGlShader::readFile(const std::string& filepath)
	{
		HZ_PROFILE_FUNCTION();


		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
			HZ_CORE_ERROR("file error: no such shader file:{0}", filepath);
		return result;

	}

	static GLenum shaderTypeFromString(const std::string& type)
	{
		HZ_PROFILE_FUNCTION();


		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "pixel" || type == "fragment")
			return GL_FRAGMENT_SHADER;

		return 0;
	}
	std::unordered_map<GLenum,std::string> OpenGlShader::preProcess(const std::string& source)
	{
		HZ_PROFILE_FUNCTION();



		const char* typeToken = "#type";

		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);

		std::unordered_map<GLenum, std::string> shaderSource;

		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);//begin from pos to find
			HZ_CORE_ASSERT(eol != std::string::npos, "Syntax error");//eol: the end pos of the line
			size_t begin = pos + typeTokenLength + 1; //the pos after "#type"
			std::string type = source.substr(begin, eol - begin);
			HZ_CORE_ASSERT(shaderTypeFromString(type), "Invalid shader type specific");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);// 第一个不是 \r\n 的位置
			pos = source.find(typeToken, nextLinePos); //下一个#type的位置
			shaderSource[shaderTypeFromString(type)] = source.substr(nextLinePos, 
				(pos == std::string::npos) ? (source.size() - nextLinePos):(pos-nextLinePos)); //感觉cherno写错了,改了下... 没有下一个就直接到底,

		}
		return shaderSource;

	}



	

}