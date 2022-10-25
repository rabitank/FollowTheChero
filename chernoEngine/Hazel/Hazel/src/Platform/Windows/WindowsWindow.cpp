#include "hzpch.h"
#include "WindowsWindow.h"
#include "Hazel/Window.h"
#include "Platform/openGl/OpenGLContext.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"
#include "Hazel/Events/KeyEvent.h"




namespace Hazel
{
	static bool s_GLFWInitialized = false;


	static void GLFWErrorCallback(int error_code, const char* description)
	{
		HZ_CORE_ERROR("GLFW Error {0}:{1}", error_code, description);

	}

	Window* Window::Create(const WindowProps& props ) { //attention : 默认参数声明过后,在定义里不要再写了,会造成重定义默认参数

		return new WindowsWindow(props);//applcation 类中会有一个指针变量来存储它
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);

	}
	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}
	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;
		m_Data.title = props.Title;


		HZ_CORE_INFO("Create Window {0}({1}, {2})", props.Title, props.Width, props.Height);
		
		if (!s_GLFWInitialized)
		{

			int success = glfwInit();
			HZ_CORE_ASSERT(success, "Could not intialize GLFW!");//ASSERT core.h定义的断言

			glfwSetErrorCallback(GLFWErrorCallback);

			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.title.c_str(),nullptr,nullptr);
		
		m_Context = new OpenGlContext(m_Window);//? m_Context 可能会是静态的,因为它可能只有一个即可 ,OpenGlContext是 GraphicContext()的派生实现类之一
		m_Context->Init();
		// ↓抽象以下代码



		glfwSetWindowUserPointer(m_Window, &m_Data); //使m_Window的用户指针(void*) 指向 m_data: m_data这里主要是传递回调函数
		SetVSync(true);//yi bu

		//SET GLFW CallBack
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)  //  回调函数形式已被glfw指定
			{
				WindowData& data =*(WindowData*)glfwGetWindowUserPointer(window);
				data.Width = width; //维持
				data.Height= height;

				WindowsResizeEvent event(width,height);
				data.EventCallback(event);

			
			}); 
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) //m_Window 会被opengl作为lambda的参数的放心
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowsCloseEvent event;
				data.EventCallback(event);


			});
		//git clone https://github.com/CompVis/stable-diffusion.git repositories/stable-diffusion

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window,int key,int scancode,int action ,int mods) 
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				
				switch (action)
				{
				case GLFW_PRESS:
				{
					
					KeyPressedEvent event(key,0); //0: repeat count
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallback(event);

					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key,1); //暂时是1
					data.EventCallback(event);
					break;
				}
				default:
					break;
				}

			});
		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode) //chartyped 把 KeyCallback挤掉了 ???,输入a~z事件类型是,glfw把它依然当作文本输入处理,charCallBack没有KeycallBack 
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				KeyTypedEvent event((int)keycode);
				data.EventCallback(event);


				//event spread: window -> application.onevent(by binding to data) -> layer.onevent in layerstack -> dispatch in layer.onevent

			});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				
				switch (action)
				{
				case GLFW_PRESS:
				{
					
					MouseButtonPressedEvent event(button); //0: repeat count
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);

					break;
				}
				
				default:
					break;
				}

			});
		
		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				
				MouseScorllEvent event((float)xoffset, (float)yoffset); //是偏移量
				data.EventCallback(event);

			});
		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				
				MouseMoveEvent event((float)xpos, (float)ypos);
				data.EventCallback(event);

			});
		

			
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}
	void WindowsWindow::onUpdate()
	{
		glfwPollEvents();
		m_Context->SwapBuffers(); //↓ 我们将会抽象它(为了将来的多平台,先从opengl抽象一套)

	}


	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);//交换间隔设为一帧 等待帧更新完毕进入下一帧,这将导致帧更新频率大于/小于屏幕刷新频率(60~70)
		else
			glfwSwapInterval(0);
		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync()const
	{
		return m_Data.VSync;
	}


}