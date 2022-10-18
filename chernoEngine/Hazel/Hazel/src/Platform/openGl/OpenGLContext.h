#pragma once
#include "Hazel/Renderer/GraphicsContext.h"
#include "GLFW/glfw3.h"

namespace Hazel
{
	class OpenGlContext:public GraphicsContext
	{
	public:
		OpenGlContext(GLFWwindow* windowHandle);
		~OpenGlContext();
		virtual void Init() override;
		virtual void SwapBuffers() override;	
	
	private:
		GLFWwindow* m_windowHandle;
	};
}