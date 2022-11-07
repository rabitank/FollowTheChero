#pragma once

#include "hzpch.h"
#include "Hazel/Core/Base.h"
#include "Hazel/Events/Event.h"

namespace Hazel
{
	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& title="Hazel Engine", uint32_t width = 1280, uint32_t height=720)
			:Title(title), Width(width), Height(height)
		{

		}


	};

	class  Window //抽象类/接口 ,没成员
	{
	public:
		using EventCallBackFn = std::function<void(Event&)>;//函数指针
		
		virtual ~Window() {};
		virtual void onUpdate() = 0;
		virtual uint32_t  GetWidth() const= 0;
		virtual uint32_t  GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallBackFn& callback) = 0; //因该有个回调函数成员
		virtual void SetViewPort(float width,float height) = 0; //因该有个回调函数成员
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;
		//暴露m_window - for input,void*是因为不一定是glfwwindow

		static Window* Create(const WindowProps& props = WindowProps());//  静态,单例成员?,例外定义
		//Create 会有适应平台的多个版本,现在还不需要实现 window.h 作为接口


	};




}