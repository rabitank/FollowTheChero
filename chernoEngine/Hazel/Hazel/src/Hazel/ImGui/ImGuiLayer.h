#pragma once


#include "Hazel/Core/Layer.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Events/MouseEvent.h"
#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/ApplicationEvent.h"

namespace Hazel
{
	class HAZEL_API ImGuiLayer :public Layer
	{ //ui 层
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void virtual OnAttach() override;
		void virtual OnDetach() override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event)override;


		virtual void Begin();//打开关闭新的Imguilayer
		virtual void End();


		void SetBlockEvent(bool set) { m_blockImGuiEvent = set; }

	/*	void OnEvent(Event& e) override;*/

//	private://这里和cherno不一样,我想在之后能改ui层的响应
// 		bool virtual OnMouseMoveEvent(MouseMoveEvent& e);//需要很多event响应 配合很多的调度器,模仿imgui的事件回调接受事件
// 		bool virtual OnMouseScorllEvent(MouseScorllEvent& e);
// 		bool virtual OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
// 		bool virtual OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
// 		bool virtual OnKeyPressedEvent(KeyPressedEvent& e);
// 		bool virtual OnKeyReleasedEvent(KeyReleasedEvent& e);
// 		bool virtual OnWindowsResizeEvent(WindowsResizeEvent& e);
// 		bool virtual OnKeyTypedEvent(KeyTypedEvent& e);//键入字符单独拎出来了


	private:
		float m_time  = 0.0f;
		bool m_blockImGuiEvent = true; //是否锁住ImGui事件?...估计以后都在ImGui里操作了..
	};
};


