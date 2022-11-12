//接口,  实现与平台相关
//实现轮询输入 , ask()
//input 是 静态全局的

//类似于 glfw的io?

#pragma once

#include "hzpch.h"
#include "Hazel/Core/Base.h"

namespace Hazel
{
	class HAZEL_API Input
	{
	public:

		static bool IsKeyPressed(int Keycode);//ask key 
		static bool IsMouseButtonPressed(int button);//ask key 
		static float GetMouseX();
		static float GetMouseY();
		static std::pair<float, float> GetMousePos();


	};



}