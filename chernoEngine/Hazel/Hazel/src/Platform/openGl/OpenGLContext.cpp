#include "hzpch.h"
#include "OpenGLContext.h"
#include "hazel/core.h"
#include <glad/glad.h> //glad 已经支持opengl ,引用的glfw中的opengl(gl可选)会引起冲突 (c 没有 pragma once??)
#include <GLFW/glfw3.h>	//glad与glfw共用gl标准,函数通用(除了扩展..),glad实际上支持的是4.6(现代高版本的)的gl


namespace Hazel
{




	OpenGlContext::OpenGlContext(GLFWwindow* windowHandle)
		:m_windowHandle(windowHandle)
	{
		HZ_CORE_ASSERT(m_windowHandle,"wimdowHandle is null")
	}

	OpenGlContext::~OpenGlContext()
	{

	}

	void OpenGlContext::Init()
	{
		glfwMakeContextCurrent(m_windowHandle); //看出init需要 GLFWwindow* 参数 
		int stauts = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress); //初始化 Glad 使用 glfw的入口
		HZ_CORE_ASSERT(stauts, "Failed to initialize Glad!");// false 触发断言
	
		HZ_CORE_INFO("OpenGl Info: ");//输出opengl正在使用的渲染设备的提供商,渲染硬件
		HZ_CORE_INFO("Vendor: {0} ", glGetString(GL_VENDOR));
		HZ_CORE_INFO("Renderer: {0} ", glGetString(GL_RENDERER));
		HZ_CORE_INFO("Version: {0} ", glGetString(GL_VERSION));
	}

	void OpenGlContext::SwapBuffers()
	{
// 		glBegin(GL_TRIANGLES);
// 		glEnd(); //这里 cherno 尝试调用 gl.h里的一些旧的直接渲染的函数但是失败了


		glfwSwapBuffers(m_windowHandle);

	}

}