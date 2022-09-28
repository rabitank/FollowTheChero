#pragma once

//  Entry point 最后的编译是在 SandBoxApp.cpp 中, int main 函数的实现 在 hazel.h 最后,因此可以不声明hazel/...h 而使用引擎


#ifdef HZ_PLATFORM_WINDOWS
#include <stdio.h>
extern Hazel::Application* Hazel::CreateApplication();//外部声明

bool func(Hazel::MouseButtonPressedEvent mos)
{

	printf("bbbbb");
	return false;
}

int main(int argc,char** argv)
{



	Hazel::Log::Init(); 
	HZ_INFO("initialized ClientLog");
	HZ_INFO("x log test: ver= {0}",5 );
	HZ_CORE_INFO("initialized CoreLog");


	printf("welcome enter hazel\n");
	auto app = Hazel::CreateApplication();
	app->Run();
	delete app;
}

#endif // HZ_PLATFORM_WINDOWS  
