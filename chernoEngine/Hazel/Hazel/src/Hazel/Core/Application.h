#pragma once

#include "Base.h"

#include "Window.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "LayerStack.h"

#include "Hazel/Core/Timestep.h"

#include "Hazel/ImGui/ImGuiLayer.h"



namespace Hazel {


	class Application   //application自带推一个imguilayer啊啊啊哭晕,用来构建imgui上下文的
	{

	public:
		Application(const std::string& windowName ="Hazel App");
		virtual ~Application();
		void Run();
		void OnEvent(Event&); //as the callback func :how we deal with the event?


		void PushLayer(Layer* layer);
		void PushOverLayer(Layer* layer);

		inline Window& GetWindow() { return *m_window; } //unique_ptr 真是搞不明白t _ t
		inline static Application& Get() { return *s_instance; } 

		void Close();
		
	private:

		//不再在客户端暴露ImGui,让Hazel持有ImGuiLayer
		 

		bool OnWindowClose(WindowsCloseEvent& e);
		bool OnWindowResize(WindowsResizeEvent& e);

		bool m_running = true;
		bool m_minimized =false;


		std::unique_ptr<Window> m_window;
		ImGuiLayer* m_ImGuiLayer;
		
		LayerStack m_LayerStack;

		static Application* s_instance; //最后还是单例了

		Timestep m_Timestep;

		float m_LastFrameTime = 0.0f;


	};
	//To be definde in CLIENT 一个接口,在客户端实现
	Application* CreateApplication();
}
