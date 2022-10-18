#include "hzpch.h"
#include "OpenGlShader.h"

#include "Glad/glad.h"

#include <sstream>
#include <fstream>
#include <string> //for pareShader

namespace Hazel
{

	OpenGlShader::OpenGlShader(const std::string& Filepath)
	{
		auto [VertexShadersrc, FragmentShadersrc] = parseShader(Filepath);

		GLint  VertexShader = glCreateShader(GL_VERTEX_SHADER);

		const GLchar* VSsource = VertexShadersrc.c_str();//& 取地址这个用法,必须是左值 指必须是被存储的变量,这样才有自己的地址,右边只是返回的一个值而已,不会被维护
		HZ_CORE_INFO("vertexshader SourceCode:{0}", VertexShadersrc.c_str());
		glShaderSource(VertexShader, 1, &VSsource, 0);
		glCompileShader(VertexShader);


		{
			GLint isCompiled;

			glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(VertexShader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(VertexShader, maxLength, &maxLength, &infoLog[0]);

				HZ_CORE_ERROR("OpenGl Log:", infoLog.data());
				HZ_CORE_ASSERT(false, "OpenGl Error: VertexShder compilate failed!");


				// Either of them. Don't leak shaders.
				glDeleteShader(VertexShader);

				// Use the infoLog as you see fit.

				// In this simple program, we'll just leave
				return;
			}
		}


		GLint  FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		const GLchar* FSsource = FragmentShadersrc.c_str();
		glShaderSource(FragmentShader, 1, &FSsource, 0);
		glCompileShader(FragmentShader);

		{
			GLint isCompiled;
			glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(FragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(FragmentShader, maxLength, &maxLength, &infoLog[0]);

				HZ_CORE_ERROR("OpenGl Log:", infoLog.data());
				HZ_CORE_ASSERT(false, "OpenGl Error: FragmentShder compilate failed!");

				// We don't need the shader anymore.
				glDeleteShader(FragmentShader);
				// Either of them. Don't leak shaders.
				glDeleteShader(VertexShader);

				// Use the infoLog as you see fit.

				// In this simple program, we'll just leave
				return;
			}
		}



		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		m_rendererID = glCreateProgram();

		// Attach our shaders to our program
		glAttachShader(m_rendererID, VertexShader);
		glAttachShader(m_rendererID, FragmentShader);

		// Link our program
		glLinkProgram(m_rendererID);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(m_rendererID, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_rendererID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_rendererID, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(m_rendererID);
			// Don't leak shaders either.
			glDeleteShader(VertexShader);
			glDeleteShader(FragmentShader);

			// Use the infoLog as you see fit.

			// In this simple program, we'll just leave

			HZ_CORE_ERROR("OpenGl Log:{0}", infoLog.data());
			HZ_CORE_ASSERT(false, "OpenGl Error: Program link shader failed!");

			return;
		}

		glValidateProgram(m_rendererID);

		// Always detach shaders after a successful link.
		glDetachShader(m_rendererID, VertexShader); //类似于 .obj文件
		glDetachShader(m_rendererID, FragmentShader);











	}

	OpenGlShader::~OpenGlShader()
	{
		glDeleteProgram(m_rendererID);
	}
	void OpenGlShader::Bind() const
	{
		glUseProgram(m_rendererID);

	};

	void OpenGlShader::UnBind() const
	{

		glUseProgram(0); // T_T	

	}

	std::tuple<std::string, std::string> OpenGlShader::parseShader(const std::string& filepath)
	{
		enum class shadertype
		{
			NONE = -1, vertex, fragment
		};

		shadertype type = shadertype::NONE;

		std::ifstream stream(filepath);
		std::stringstream ss[2];
		std::string line;
		while (getline(stream, line))
		{
			if (line.find("#shader") != std::string::npos)//不等于无效位置
			{
				if (line.find("vertex") != std::string::npos)
					//todo:set mode to vertex
					type = shadertype::vertex;
				else if (line.find("fragment") != std::string::npos)
					//...
					type = shadertype::fragment;
			}
			else
			{
				ss[(int)type] << line << "\n";

			}
		}

		return { ss[0].str(),ss[1].str() };

	}

}