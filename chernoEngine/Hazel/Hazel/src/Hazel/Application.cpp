#include "Application.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Log.h"

namespace Hazel 
{

	Application::Application()
	{

	}

	Application::~Application()
	{

	}
	void Application::Run()
	{


		WindowsResizeEvent e(1280, 720);
		if (e.IsInCategory(EventCategoryInput))
		{
		HZ_INFO(e); //spdlog需要引入 fmt/ostr.h 才允许自定义类型(修改字符串流运算符<<)

		}
		if (e.IsInCategory(EventCategoryApplication))
		{
		HZ_INFO(e); //spdlog需要引入 fmt/ostr.h 才允许自定义类型(修改字符串流运算符<<)

		}
		 //先看完再尝试运行....QAQ
	}
}