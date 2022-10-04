#pragma once
#include "Hazel/Window.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ObjIdl.h>


#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>


namespace Hazel
{
	class WindowsWindow:public Window
	{	

	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void onUpdate() override; //application调用 ,每帧执行 ,GLFW询问事件
		virtual unsigned int  GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		inline void SetEventCallback(const EventCallBackFn& Callbackfn)override { m_Data.EventCallback = Callbackfn; };
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

	private:
		GLFWwindow* m_Window;
		struct WindowData
		{
			std::string title;
			unsigned int Width;
			unsigned int Height;

			bool VSync;

			EventCallBackFn EventCallback;
		};
		WindowData m_Data;
	};

}

