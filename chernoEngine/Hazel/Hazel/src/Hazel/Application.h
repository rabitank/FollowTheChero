#pragma once

#include "core.h"

#include "Window.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "LayerStack.h"

#include "Hazel/ImGui/ImGuiLayer.h"

#include "Renderer/Shader.h"
#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"

	

namespace Hazel {


	class HAZEL_API Application
	{

	public:
		Application();
		virtual ~Application();
		void Run();
		void OnEvent(Event&); //as the callback func :how we deal with the event?


		void PushLayer(Layer* layer);
		void PushOverLayer(Layer* layer);

		inline Window& GetWindow() { return *m_window; } //unique_ptr 真是搞不明白t _ t
		inline static Application& Get() { return *m_instance; } 

		
	private:

		//不再在客户端暴露ImGui,让Hazel持有ImGuiLayer
		 

		bool OnWindowClose(WindowsCloseEvent& e);
		bool m_running = true;


		std::unique_ptr<Window> m_window;
		ImGuiLayer* m_ImGuiLayer;
		
		LayerStack m_LayerStack;

		static Application* m_instance; //最后还是单例了

		std::shared_ptr<VertexArray> m_VertexArray;  // VB,IB现在由 VA持有
		std::shared_ptr<Shader> m_shader; //换shared_ptr了...大逃杀模式...// shader...,跟着application吧,你是唯一的 --中二病
		std::shared_ptr<VertexArray> m_SquraVA; //for test

	};
	//To be definde in CLIENT 一个接口,在客户端实现
	Hazel::Application* CreateApplication();
}
