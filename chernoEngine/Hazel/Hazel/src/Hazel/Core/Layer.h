#pragma once
#include "Hazel/Core/Base.h"
#include "Hazel/Events/Event.h"

#include "Hazel/Core/Timestep.h"


namespace Hazel
{
	class HAZEL_API Layer
		/// Layer 是一种栈概念 , 负责渲染该层图像,处理该层事件,传递/响应/终止事件传递,层将在 LayerStack上根据调用顺序依次渲染并覆盖
	{
	public:
		Layer(const std::string & = "Layer"); //


		//等待继承与重写
		virtual ~Layer();

		virtual void OnAttach() {};// 即 当Layer 被 推到 layerStack 上时使用
		virtual void OnDetach() {};// 即		 从 layerStack放 下 时 使用
		virtual void OnUpdate(Timestep time) {};// 层更新时使用 ,每帧调用
		virtual void OnImGuiRender() {};
		virtual void OnEvent(Event& event) {};

		inline const  std::string& Getname() { return m_Debugname; }

	protected:
		std::string m_Debugname; //暂时以输入名字创建层实例
	};

}


