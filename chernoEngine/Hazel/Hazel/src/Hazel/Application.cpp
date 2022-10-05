#include "hzpch.h"

#include "Application.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Log.h"

#include <glad/glad.h>
//设置宏后,glfw已经不包含gl了,需要glad引用gl头来为其函数提供实现

#include "Hazel/Input.h"
 
namespace Hazel 
{

#define BIND_EVENT_FN(x)  std::bind(&Application::x,this,std::placeholders::_1)

	Application* Application::m_instance = nullptr; //application 成为单例模式

	Application::Application()
	{
		HZ_CORE_ASSERT(!(m_instance) ,"Application has been exists") // 不存在 , 反之停下

		m_instance = this;
		
		m_window = std::unique_ptr<Window>(Window::Create());//这里因该是windowswindowcpp实现的
		//make_unique<>() 对于我来说还是一个十分玄幻的东西,<_tp> 错误还是 unique_ptr吧

		m_window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		unsigned int id;
		glGenVertexArrays(1, &id);

	}
	

	void Application::OnEvent(Event&e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.DisPatch<WindowsCloseEvent>(BIND_EVENT_FN(OnWindowClose));//bind : the first parameter is waitting
			//dispatcher, if you see WindowCloseEvent ,then use this callfuction with your event e

		//HZ_CORE_TRACE("{0}",e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			// vector 结构应该是 begin空 1 2 3 4 5 end空
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;

		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverLayer(Layer* layer)
	{
		m_LayerStack.PushOverLayer(layer);
		layer->OnAttach();
	}

	Application::~Application()
	{

	}
	void Application::Run()
	{


// 		WindowsResizeEvent e(1280, 720);
// 
// 		if (e.IsInCategory(EventCategoryInput))
// 		{
// 		HZ_INFO(e); //spdlog需要引入 fmt/ostr.h 才允许自定义类型(修改字符串流运算符<<)
// 
// 		}
// 		if (e.IsInCategory(EventCategoryApplication))
// 		{
// 		HZ_INFO(e); //spdlog需要引入 fmt/ostr.h 才允许自定义类型(修改字符串流运算符<<)
// 
// 		}
		 //先看完再尝试运行....QAQ

		while (m_running)
		{
			glClearColor(0.01f, 0.2f, 0.7f, 1.0f); //现在由glad提供该函数
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			//轮询测试
			/*auto [x, y] = Input::GetMousePos(); */
			//如果查询按键,我们不应该依赖glfw的宏来输入keycode,这会导致兼容性问题	 
			/*HZ_CORE_TRACE("{0},{1}", x, y);*/

			m_window->onUpdate();
		};
	}

	bool Application::OnWindowClose(WindowsCloseEvent& e)
	{
		m_running = false;
		return true;
	}
}
		