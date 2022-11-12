
#include "hzpch.h" //有了预编译头,项目设置后每个cpp都要用的,欸
#include <GLFW/glfw3.h>

#include "Hazel/Core/Application.h"
#include "Hazel/Core/Input.h"

namespace Hazel
{




	bool Input::IsKeyPressed(int keycode) //只是在类外写实现不用写override啦,
	{//需要获取window的m_window来询问
		//如果要引用的话需要继承const
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		
		auto state  = glfwGetKey(window, keycode); //keycode>38
		//glfw帮你问状态

		return state == GLFW_PRESS || state == GLFW_REPEAT;
		
	
	}

	bool Input::IsMouseButtonPressed(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetMouseButton(window, button);

		return state == GLFW_PRESS;

	}

	std::pair<float, float> Input::GetMousePos()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;

		glfwGetCursorPos(window, &xpos, &ypos);

		return { (float)xpos,(float)ypos };
	}

	float Input::GetMouseX()
	{
		auto[xpos, ypos] = GetMousePos();
		return xpos;

	}

	float Input::GetMouseY()
	{
		
		auto[xpos, ypos] = GetMousePos();
		return ypos;

	}

}
