#pragma once


#include "Layer.h"


namespace Hazel
{
	class LayerStack
	{
	private:
		std::vector<Hazel::Layer*> m_layerstack;
		unsigned int m_LayerInsertIndex =0	; //类似pin指针,维护推送层尾部下标???用于反向遍历或调用渲染更新某个Layer
		
	public:
		LayerStack();
		~LayerStack();

		////推送层(push) _ 覆盖层(over) ,覆盖层永远在推送层之上,m_layerInsert指推送层末尾空位,进行推送层插入
		//我们从vector第一个begin 渲染到 end
		void PushLayer(Layer* layer ); //由 layerInsert指定插入位置
		void PushOverLayer(Layer* layer );
		void PopLayer( Layer* layer );
		void PopOverLayer( Layer* layer );

		std::vector<Hazel::Layer*>::iterator begin() { return m_layerstack.begin(); }
		std::vector<Hazel::Layer*>::iterator end() { return m_layerstack.end(); }

	};
}



