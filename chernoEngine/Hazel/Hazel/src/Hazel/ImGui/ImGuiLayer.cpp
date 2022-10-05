
#include "hzpch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "Platform/openGl/ImGuiOpenGlRenderer.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>


#include "Hazel/Application.h"


namespace Hazel
{

	ImGuiLayer::ImGuiLayer()
		:Layer("ImGuiLayer"),m_time((float)glfwGetTime()) //初始化 调用父类构造 m_time 初始化与cherno不同 p15	
	{

	}

	ImGuiLayer::~ImGuiLayer()
	{

	}


	void ImGuiLayer::OnAttach() //挂到栈上准备渲染了
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGuiIO& io = ImGui::GetIO(); //获得imgui的 io流  进行设置
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;// |= 按位或并赋值符 io 添加收录事件吧 
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
	
		//temporary
		//累死我了! 草你! cherno 
		io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB; //键位映射:暂时收录这么多的键位
		io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;	
		io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
		io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
		io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
		io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
		io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
		io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
		io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

		ImGui_ImplOpenGL3_Init("#version 410"); 
	}

	void ImGuiLayer::OnDetach()
	{

	}

	void ImGuiLayer::OnUpdate()
	{
		

		ImGuiIO& io = ImGui::GetIO();
		
		Application& app = Application::Get(); //get singal instance
		Window& app_window = app.GetWindow();
		io.DisplaySize = ImVec2(app_window.GetWidth(), app_window.GetHeight());

		float time = (float)glfwGetTime(); //获得时间
		io.DeltaTime= m_time > 0.0 ? (time - m_time):(1.0f / 60.0f); //获得	帧间时间

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame(); //先设尺寸,后画帧??

		static bool show =true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());	

		//接下来 imguiLayer的onevent();
	}

	void ImGuiLayer::OnEvent(Event& e)
	{

		EventDispatcher diapatcher(e);
		switch (e.GetEventType())
		{//没必要switch 但是我就要
		
		case EventType::MouseMoved:
			
			diapatcher.DisPatch<MouseMoveEvent>(BIND_EVENT_CALLFN(ImGuiLayer::OnMouseMoveEvent));
			break;
		case EventType::MouseButtonPressed :
			 
			diapatcher.DisPatch<MouseButtonPressedEvent>(BIND_EVENT_CALLFN(ImGuiLayer::OnMouseButtonPressedEvent));
			break;
		case EventType::MouseButtonReleased:
			 
			diapatcher.DisPatch<MouseButtonReleasedEvent>(BIND_EVENT_CALLFN(ImGuiLayer::OnMouseButtonReleasedEvent));
			break;
		case EventType::MouseScrolled:
			 
			diapatcher.DisPatch<MouseScorllEvent>(BIND_EVENT_CALLFN(ImGuiLayer::OnMouseScorllEvent));
			break;
		case EventType::KeyPressed:
			 
			diapatcher.DisPatch<KeyPressedEvent>(BIND_EVENT_CALLFN(ImGuiLayer::OnKeyPressedEvent));
			break;
		case EventType::KeyReleased:
			 
			diapatcher.DisPatch<KeyReleasedEvent>(BIND_EVENT_CALLFN(ImGuiLayer::OnKeyReleasedEvent));
			break;
		case EventType::WindowResize:
			 
			diapatcher.DisPatch<WindowsResizeEvent>(BIND_EVENT_CALLFN(ImGuiLayer::OnWindowsResizeEvent));
			break;
		case EventType::KeyTyped:
			diapatcher.DisPatch<KeyTypedEvent>(BIND_EVENT_CALLFN(ImGuiLayer::OnKeyTypedEvent));
			break;
		default:
			break;

		}


	}


	bool ImGuiLayer::OnMouseMoveEvent(MouseMoveEvent& e) //模仿Imgui原生的事件回调
	{

		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(e.GetX(), e.GetY());

		return false;
	}

	bool ImGuiLayer::OnMouseScorllEvent(MouseScorllEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH+=  e.GetXOffset();
		io.MouseWheel += e.GetYOffset();


		return true;
	}

	bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButtonCode()] =true;
		return false; //希望继续穿透
	}

	bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButtonCode()] = false;
		return false; //希望继续穿透
	}

	bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();

		//一些关键键被另外保存了(主要是有因为两个)

		io.KeysDown[e.GetKeyCode()] = true;
		io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER]; // windows 键 mac 键
		io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
		io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
		return true;
	}

	bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = false;
		io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
		return true;
	}

	bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		int keycode = e.GetKeyCode();

		if (keycode > 0 && keycode < 0x100000)
			io.AddInputCharacter((unsigned int)keycode);//输入文本需要这个,0.0

		return false;
	}


	bool ImGuiLayer::OnWindowsResizeEvent(WindowsResizeEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(e.GetWeight(), e.GetHeight());//和update重复了
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f); //layer渲染大小:沾满displaysize
		glViewport(0, 0, e.GetWeight(), e.GetHeight()); //令gl渲染视口
		return true;
	}

}