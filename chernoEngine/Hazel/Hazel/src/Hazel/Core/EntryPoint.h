#pragma once
#include "Hazel/Core/Base.h"
//  Entry point 最后的编译是在 SandBoxApp.cpp 中, int main 函数的实现 在 hazel.h 最后,因此可以不声明hazel/...h 而使用引擎


#ifdef HZ_PLATFORM_WINDOWS
#include <stdio.h>
extern Hazel::Application* Hazel::CreateApplication();//外部声明



int main(int argc,char** argv)
{



	Hazel::Log::Init(); 

	HZ_PROFILE_BEGIN_SESSION("StartUp","HazelProfile-Startup.json");
	auto app = Hazel::CreateApplication();
	HZ_PROFILE_END_SESSION();


	HZ_PROFILE_BEGIN_SESSION("Run","HazelProfile-Runtime.json");
	app->Run();
	HZ_PROFILE_END_SESSION();

	HZ_PROFILE_BEGIN_SESSION("Run","HazelProfile-ShutDown.json");
	delete app;
	HZ_PROFILE_END_SESSION();
}

#endif // HZ_PLATFORM_WINDOWS  
