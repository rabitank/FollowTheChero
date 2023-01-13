#include "hzpch.h"
#include "OpenGlShader.h"
#include <fstream>

#include "Glad/glad.h"
#include <glm/gtc/type_ptr.hpp>
#include "Hazel/Core/Timer.h"


#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

namespace Hazel
{
	namespace Utils {

		static GLenum ShaderTypeFromString(const std::string& type)
		{
			if (type == "vertex")
				return GL_VERTEX_SHADER;
			if (type == "fragment" || type == "pixel")
				return GL_FRAGMENT_SHADER;

			HZ_CORE_ASSERT(false, "Unknown shader type!");
			return 0;
		}

		static shaderc_shader_kind GLShaderStageToShaderC(GLenum stage)
		{
			switch (stage)
			{
			case GL_VERTEX_SHADER:   return shaderc_glsl_vertex_shader;
			case GL_FRAGMENT_SHADER: return shaderc_glsl_fragment_shader;
			}
			HZ_CORE_ASSERT(false, "Unknown shader stage!");
			return (shaderc_shader_kind)0;
		}

		static const char* GLShaderStageToString(GLenum stage)
		{
			switch (stage)
			{
			case GL_VERTEX_SHADER:   return "GL_VERTEX_SHADER";
			case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
			}
			HZ_CORE_ASSERT(false, "Unknown shader stage!");
			return nullptr;
		}

		static const char* GetCacheDirectory()
		{
			// TODO: make sure the assets directory is valid
			return "assets/cache/shader/opengl";
		}

		static void CreateCacheDirectoryIfNeeded()
		{
			std::string cacheDirectory = GetCacheDirectory();
			if (!std::filesystem::exists(cacheDirectory))
				std::filesystem::create_directories(cacheDirectory);
		}

		static const char* GLShaderStageCachedOpenGLFileExtension(uint32_t stage)
		{
			switch (stage)
			{
			case GL_VERTEX_SHADER:    return ".cached_opengl.vert";
			case GL_FRAGMENT_SHADER:  return ".cached_opengl.frag";
			}
			HZ_CORE_ASSERT(false, "Unknown shader stage!");
			return "";
		}

		static const char* GLShaderStageCachedVulkanFileExtension(uint32_t stage)
		{
			switch (stage)
			{
			case GL_VERTEX_SHADER:    return ".cached_vulkan.vert";
			case GL_FRAGMENT_SHADER:  return ".cached_vulkan.frag";
			}
			HZ_CORE_ASSERT(false, "Unknown shader stage!");
			return "";
		}


	}



	OpenGlShader::OpenGlShader(const std::string& Filepath)
		:m_filePath(Filepath)
	{
		HZ_PROFILE_FUNCTION();

		Utils::CreateCacheDirectoryIfNeeded();

		std::string source = readFile(Filepath);
		auto shaderSources = preProcess(source);
		{
			Timer timer;
			CompileOrGetVulkanBinaries(shaderSources);
			CompileOrGetOpenGLBinaries();
			CreateProgram();
			HZ_CORE_WARN("Shader creation took {0} ms", timer.ElapsedMillis());
		}


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
				HZ_CORE_ASSERT(false, "OpenGl Error: compilate failed!");

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

	void OpenGlShader::CompileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		GLuint program = glCreateProgram();

		shaderc::Compiler compiler;  //spirv 编译器 , shaderc: shadersource::
		shaderc::CompileOptions options; 
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2); //配置目标编译环境 , 目标环境版本

		const bool optimize = true;
		if (optimize)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);
		//返回存放二进制spirv编译结果的目录 (cache/opengl)
		std::filesystem::path cacheDirectory = Utils::GetCacheDirectory(); //返回存储shader的目录

		auto& shaderData = m_VulkanSPIRV; //输出方式,引用赋值 m_VulkanSPIRV将会存放多个编译出的多个二进制shader 的字节码
		
		shaderData.clear();
		for (auto&& [stage, source] : shaderSources) //stage理解为阶段或种类 有.vert 顶点与 .frag 片元
		{
			std::filesystem::path shaderFilePath = m_filePath;

			//拼接shader输出路径
			//shaderFilePath.filename()似乎直接返回路径指的文件名
			std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + Utils::GLShaderStageCachedVulkanFileExtension(stage)); 
			
			//ifstream in (ios::in 读出 ios::binary 二进制字节码)
			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				//放置光标(你可以这么理解,放到end ,相对位置 offset =0)
				in.seekg(0, std::ios::end);
				auto size = in.tellg();//告诉光标位置

				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];//引用然后 写入到shaderData
				//resize干嘛??
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			//如果不能in(不存在Vulkan编译后的文件),那么现场编译,shaderData的内容直接放热乎的。 ->这次的Bug原因是,上次失败的编译文件(为空)没有清楚,可以读,就没有现场编译,但是内容是空的
			//修复就是,失败了要删除文件...但是失败了不会新建文件的啊??空文件是怎么产生的??
			else
			{
				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::GLShaderStageToShaderC(stage), m_filePath.c_str(), options);
				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					HZ_CORE_ERROR(module.GetErrorMessage());
					HZ_CORE_ASSERT(false);
				}

				shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
				if (out.is_open())
				{
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}

		for (auto&& [stage, data] : shaderData)
			Reflect(stage, data);
	}

	void OpenGlShader::CompileOrGetOpenGLBinaries()
	{
		auto& shaderData = m_OpenGLSPIRV;

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
		const bool optimize = false;
		if (optimize)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);

		std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();

		shaderData.clear();
		m_OpenGLSourceCode.clear();
		for (auto&& [stage, spirv] : m_VulkanSPIRV)
		{
			std::filesystem::path shaderFilePath = m_filePath;
			std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + Utils::GLShaderStageCachedOpenGLFileExtension(stage));

			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			else
			{
				spirv_cross::CompilerGLSL glslCompiler(spirv);
				m_OpenGLSourceCode[stage] = glslCompiler.compile();
				auto& source = m_OpenGLSourceCode[stage];

				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::GLShaderStageToShaderC(stage), m_filePath.c_str());
				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					HZ_CORE_ERROR(module.GetErrorMessage());
					HZ_CORE_ASSERT(false);
				}
				//我们把编码结果(uint32字节码可迭代容器)输出
				shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
				if (out.is_open())
				{
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}
	}


	//CreateProgram , more step shaders -> truly rendererpipe
	void OpenGlShader::CreateProgram()
	{
		GLuint program = glCreateProgram();

		std::vector<GLuint> shaderIDs;
		for (auto&& [stage, spirv] : m_OpenGLSPIRV)
		{
			GLuint shaderID = shaderIDs.emplace_back(glCreateShader(stage));

			//3:binaryFormat :指定二进制文件中包含的着色器二进制文件的格式
			//4:binary :指定包含预编译二进制着色器代码的字节数组的地址。
			//5:length :指定其地址以二进制形式给出的数组的长度。-> 字节码大小为 4个字节
			glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), spirv.size() * sizeof(uint32_t));

			//必须指定要使用的入口点(执行开始)，并为该入口点使用的任何专用化常量提供值。这是通过单个函数完成的
			glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);
			glAttachShader(program, shaderID);
		}

		glLinkProgram(program);

		GLint isLinked;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());
			HZ_CORE_ERROR("Shader linking failed ({0}):\n{1}", m_filePath, infoLog.data());

			glDeleteProgram(program);

			for (auto id : shaderIDs)
				glDeleteShader(id);
		}


		//解除连接后删除中间编译结果(像obj一样),只保留链接完成的program
		for (auto id : shaderIDs)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}

		m_rendererID = program;
	}

	//似乎只是打印shader信息? 或者理解为模仿GPUshader解析后的信息显示?
	void OpenGlShader::Reflect(GLenum stage, const std::vector<uint32_t>& shaderData)
	{
		spirv_cross::Compiler compiler(shaderData);
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();

		HZ_CORE_TRACE("OpenGLShader::Reflect - {0} {1}", Utils::GLShaderStageToString(stage), m_filePath);
		HZ_CORE_TRACE("    {0} uniform buffers", resources.uniform_buffers.size());
		HZ_CORE_TRACE("    {0} resources", resources.sampled_images.size());

		HZ_CORE_TRACE("Uniform buffers:");
		for (const auto& resource : resources.uniform_buffers)
		{
			const auto& bufferType = compiler.get_type(resource.base_type_id);
			uint32_t bufferSize = compiler.get_declared_struct_size(bufferType);
			uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
			int memberCount = bufferType.member_types.size();

			HZ_CORE_TRACE("  {0}", resource.name);
			HZ_CORE_TRACE("    Size = {0}", bufferSize);
			HZ_CORE_TRACE("    Binding = {0}", binding);
			HZ_CORE_TRACE("    Members = {0}", memberCount);
		}
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


	std::unordered_map<GLenum, std::string> OpenGlShader::preProcess(const std::string& source)
	{
		HZ_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0); //Start of shader type declaration line
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos); //begin from pos to find
			HZ_CORE_ASSERT(eol != std::string::npos, "Syntax error");//eol: the end pos of the line
			size_t begin = pos + typeTokenLength + 1; //the pos after "#type"
			std::string type = source.substr(begin, eol - begin);
			HZ_CORE_ASSERT(Utils::ShaderTypeFromString(type), "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
			HZ_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
			pos = source.find(typeToken, nextLinePos); //Start of next shader type declaration line

			shaderSources[Utils::ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSources;

	}









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






	

}