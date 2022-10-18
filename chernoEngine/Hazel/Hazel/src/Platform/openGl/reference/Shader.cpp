#include "Shader.h"
#include <fstream> //for read file
#include <string> //for getline
#include <sstream> // for string stream
#include "Hazel/Renderer/renderer.h"


namespace Hazel
{
	
	
	
	Shader::Shader(const std::string& CodeFilePath)
		:m_Filepath(CodeFilePath),m_renderer(0)
	{
		auto[vertexshader, fragmentshader]= parseShader(CodeFilePath); //vsdebug时相对部分来自工作目录(.vcxproj)
		m_renderer = createshader(vertexshader, fragmentshader);
	
	}
	
	Shader::~Shader()
	{
		GLCall(glDeleteProgram(m_renderer));
	}
	
	void Shader::Bind() const
	{
		GLCall(glUseProgram(m_renderer));
	
	}
	
	void Shader::Unbind()const
	{
		GLCall(glUseProgram(0));
	
	}
	
	std::tuple<std::string, std::string> Shader::parseShader(const std::string& filepath)
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
	
	unsigned int Shader::createshader(const std::string& vertexshader, const std::string& fragmentshader)
	{
		//把源码当作字符串传进来,键结到一个着色器程序,并返回那个程序(shader)的id
		GLCall(unsigned int program = glCreateProgram());
		unsigned int vs = CompileShader(vertexshader, GL_VERTEX_SHADER);//0xb831就是一个16位int
		unsigned int fs = CompileShader(fragmentshader, GL_FRAGMENT_SHADER);//compile就是生成中间的shader文件
		//你在远程操控GPU,原始的GPU,控制写入源码,甚至需要手动编译链接
		glAttachShader(program, vs);
		glAttachShader(program, fs);//我们的管线有了两个着色器
		glLinkProgram(program);
		glValidateProgram(program);//validate 批准,使有效;我们已经链接并激活
	
		glDeleteShader(vs);//链接到program后,我们已经不需要中间的shader文件(源码)了
		glDeleteShader(fs);//不过cherno不喜欢,这样不好调试,而且消耗也不高
	
		return program;
	}
	
	void Shader::SetUniform4f(const std::string name, float v0, float v1, float v2, float v3)
	{
		GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
	
	}
	void Shader::SetUniformMat4f(const std::string name, const glm::mat4& mat)
	{
		GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]));//4f v : vector/array
		//location
		//count:1
		//是否需要转置(为列排布)
		//float地址
	
	}
	void Shader::SetUniform1f(const std::string name, float v0)
	{
		GLCall(glUniform1f(GetUniformLocation(name), v0));
	
	}
	void Shader::SetUniform1i(const std::string name, int v0)
	{
		GLCall(glUniform1i(GetUniformLocation(name), v0));
	
	}
	void Shader::SetUniform1iv(const std::string name,int size,int* v)
	{
		GLCall(glUniform1iv(GetUniformLocation(name),size,v));
	}
	
	int Shader::GetUniformLocation(const std::string& name) const
	{
		if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
			return m_UniformLocationCache[name];
	
		GLCall(int u_color_location = glGetUniformLocation(m_renderer,name.c_str()));
		//c_str()函数返回一个指向正规C字符串的指针常量,与c兼容,同时与 const GLchar* 存在转换
		//ASSERT(u_color_location != -1); //断言
		//不断言是因为, -1,找不到有时也有用
		if (u_color_location == -1)
			std::cout << "warning: Uniform "<<'*'<< name <<'*'<< " don't exist " << std::endl;
		
		m_UniformLocationCache[name] = u_color_location;
		return u_color_location;
	
	}
	
	unsigned int  Shader::CompileShader(const std::string& source, unsigned int type)
	{
		GLCall( unsigned int id = glCreateShader(type));
		const char* src = source.c_str(); //返回一个以null结尾的不可变数组的指针,如果你的source坏掉了,src也无效
		GLCall(glShaderSource(id, 1, &src, NULL));
		GLCall(glCompileShader(id));
		//向shader里填充源码 (id , 
		//源码字符串数量, 
		//源码string头指针的指针(因为有时候是字符串数组) , 
		//一个长度数组指针,用于指定对应字符串读取长度(Null默认以空结尾,全都一直读完),<=0表示全读完读到0)
		//TODO:cpmile error
		int result;
		GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result)); //得到shader编译程序状态, i int(id)? v :一个矢量(数组)
		if (result == GL_FALSE)
		{
	
			int length;
			GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
			//char message[length]; //length是一个变量,可以在堆上用它分配内存,但栈上不能直接这样
			char* message = (char*)alloca(length * sizeof(char)); //cherno秘方,alloca c函数,允许在栈上动态分配内存
			GLCall(glGetShaderInfoLog(id, length, &length, message));//获得log;
			std::cout << "failed in " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader compiling" << std::endl;
			std::cout << message << std::endl;
			GLCall(glDeleteShader(id));
			return 0;
		}
		return id;
	}
}

