#include "Hazel.h"
//app 对引擎启动细节不感兴趣, 引擎入口(main)应该由引擎定义, app只需要通过实现来控制
//没有正确的程序入口点 是 dll没有更新,lib提供的函数入口与旧dll不对应引起的


class ExampleLayer :public Hazel::Layer
{
public:
	ExampleLayer()
		:Layer("Example")
	{

	}

	void OnUpdate() override
	{
		HZ_INFO("ExampleLayer::Updata");
	}

	void OnEvent(Hazel::Event& e) override
	{
		HZ_TRACE("{0}", e);
	}
};


class SandBox:public Hazel::Application
{
public:
	SandBox()
	{
		PushLayer(new ExampleLayer());// ExampleLayer* -->Layer* 
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