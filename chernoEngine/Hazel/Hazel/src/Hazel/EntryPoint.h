#pragma once

#ifdef HZ_PLATFORM_WINDOWS
#include <stdio.h>
extern Hazel::Application* Hazel::CreateApplication();//Íâ²¿ÉùÃ÷

int main(int argc,char** argv)
{
	printf("welcome enter hazel\n");
	auto app = Hazel::CreateApplication();
	app->Run();
	delete app;
}

#endif // HZ_PLATFORM_WINDOWS
