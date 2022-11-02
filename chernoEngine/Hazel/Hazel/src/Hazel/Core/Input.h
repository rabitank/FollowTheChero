//接口,  实现与平台相关
//实现轮询输入 , ask()
//input 是 静态全局的

//类似于 glfw的io?

#pragma once

#include "hzpch.h"
#include "Hazel/Core/core.h"

namespace Hazel
{
	class HAZEL_API Input
	{
	public:

		inline static bool IsKeyPressed(int Keycode) //ask key 
		{
			return s_Instance->IsKeyPressedImple(Keycode); //交由该函数接口版本实现
		};
		 inline static bool IsMouseButtonPressed(int button) //ask key 
		{
			return s_Instance->IsMouseButtonPressedImple(button);
		};
		 inline static float GetMouseX() { return s_Instance->GetMouseXImple(); }
		 inline static float GetMouseY() { return s_Instance->GetMouseYImple(); }
		 inline static std::pair<float, float> GetMousePos() { return s_Instance->GetMousePosImple();}
	private:
		static Input* s_Instance; //单例模式
	protected:
		virtual bool IsKeyPressedImple(int keycode) = 0;
		virtual bool IsMouseButtonPressedImple(int button) = 0;
		virtual float  GetMouseXImple() = 0;
		virtual float  GetMouseYImple() = 0;
		virtual std::pair<float,float> GetMousePosImple() = 0;


	};



}