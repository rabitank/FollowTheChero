#pragma once

#include<GL\glew.h>
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

#define LOG(x) std::cout<<x<<std::endl

//use MSVC lib for 断言debugger

#define ASSERT(x) if(!(x)) __debugbreak()
#define GLCall(x) GlClearError();x;ASSERT(GlLogCall(#x,__FILE__,__LINE__)); //#x ,把x变为字符串,后两个是所有编译器支持的
//到处用,防错
//对于链接错误 , 找不到/无法解析外部符号时 可以搜索该函数名在哪个库里再添加到项目即可
//opengl 的实现由显卡制造商提供 ,glfw只是提供了opengl的规范(常量,函数声明,上下文实现等),但是其实现需要访问显卡驱动 , windows使用的原生图像渲染是 direct3d 
//这里有一些额外的(更多的)库能让我们使用opengl(更新的更多功能): GLEW opengl额外扩展管理(使用前需要初始化glewInit())

//现代的opengl比较灵活底层 , 为了使用它 我们用了 GLEW

void GlClearError();

bool GlLogCall(const char* function, const char* file, int line);


class renderer//可以写成静态类,但是cherno为了多线程写成可实例的
{
public:
	void Drawer(const VertexArray& va,const IndexBuffer& ib,const Shader& shader)const;
	void clear() const;
};

