#include "Hazel.h"
//app 对引擎启动细节不感兴趣, 引擎入口(main)应该由引擎定义, app只需要通过实现来控制
//没有正确的程序入口点 是 dll没有更新,lib提供的函数入口与旧dll不对应引起的
#include "imGui/imgui.h"
// #include "imGui/backends/imgui_impl_opengl3.h"
// #include "imGui/backends/imgui_impl_glfw.h"

class ExampleLayer :public Hazel::Layer
{
public:
	ExampleLayer()
		:Layer("Example")
	{




	}
	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();
	}

	void OnUpdate() override
	{
		//HZ_INFO("ExampleLayer::Updata");
		if (Hazel::Input::IsKeyPressed(HZ_KEY_C)) 
		{
			HZ_TRACE(" c is pressed");
		}
	}

	void OnEvent(Hazel::Event& e) override
	{
	

		if (e.GetEventType() == Hazel::EventType::KeyPressed)
		{
			Hazel::KeyPressedEvent& ep = (Hazel::KeyPressedEvent&) e;
			HZ_TRACE("{0}", (char)ep.GetKeyCode());
		}

	}
};


class SandBox:public Hazel::Application
{
public:
	SandBox()
	{
	//	PushOverLayer(new Hazel::ImGuiLayer()); //重复推了ImGUILayer!!!尼玛!!!
		PushOverLayer(new ExampleLayer());// ExampleLayer* -->Layer* 
	
	}
	~SandBox()
	{

	}
};

// a box about how the game work would be send/trans to engine's entry point

Hazel::Application* Hazel::CreateApplication()
{
	return new SandBox;	

}