#include "hzpch.h"

#include "Application.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Log.h"

//设置宏后,glfw已经不包含gl了,需要glad引用gl头来为其函数提供实现

#include "Hazel/Input.h"
#include "Hazel/Renderer/Renderer.h"

#include <GLFW/glfw3.h>

namespace Hazel 
{

#define BIND_EVENT_FN(x)  std::bind(&Application::x,this,std::placeholders::_1) //主要是为了像lambda一样有个待填的参数空位




	Application* Application::m_instance = nullptr; //application 成为单例模式

	Application::Application()
	{
		HZ_CORE_ASSERT(!(m_instance) ,"Application has been exists") // 不存在 , 反之停下

		m_instance = this;
		
		m_window = std::unique_ptr<Window>(Window::Create());//这里因该是windowswindowcpp实现的
		//make_unique<>() 对于我来说还是一个十分玄幻的东西(似乎自动调用构造函数??),<_tp> 错误还是 unique_ptr吧
		m_ImGuiLayer = new ImGuiLayer; //这里选用原始指针,避免uniqueptr导致被删除

		m_window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		Renderer::Init(); //初始化 Renderer的各项设置

		PushOverLayer(m_ImGuiLayer);

		
		//牢记 gl是个状态机!!!!!!!!!,顺序重要


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
		//这就是一帧

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
			float time = (float) glfwGetTime(); //glfw 里的
			Timestep  timestep =  time -m_LastFrameTime;
			m_LastFrameTime = time;

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(timestep);

			//这个块最后会拆分到 渲染线程 里执行
			m_ImGuiLayer->Begin(); //构造ImGuiLayer上下文  牢记,unique_ptr是指针!主动 ->
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End(); 
			
								   
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
		