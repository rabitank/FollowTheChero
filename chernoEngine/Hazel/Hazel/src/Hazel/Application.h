#pragma once
#include "core.h"

namespace Hazel {


	class HAZEL_API Application
	{

	public:
		Application();
		virtual ~Application();
		void Run();
	};
	//To be definde in CLIENT 一个接口,在客户端实现
	Hazel::Application* CreateApplication();
}
