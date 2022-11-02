#pragma once



#include "GLFW/glfw3.h" // glad 已经同过定义宏 告知了glfw去除gl引用 ,另外,项目里也已经预定义了gl相关,良心glfw,低素质glad

#include "Hazel/Core/Window.h"
#include "hazel/Renderer/GraphicsContext.h"

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
		virtual void SetViewPort(float width, float height) override ;
		bool IsVSync() const override;

		inline virtual void* GetNativeWindow() const override { return m_Window; } ;
		//virtual 为了后人!!
		//不...其实 基类(或派生类)虚函数派生的函数结构上的所有函数都是虚函数(使用基类/多态类指针时多态起作用).... 这只是是提醒
		
		// window的使用实例在 静态的application里,想访问的话可以通过application单例访问
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

	private:
		GLFWwindow* m_Window;
		GraphicsContext* m_Context;


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

