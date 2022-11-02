#include "hzpch.h"
#include "LayerStack.h"

namespace Hazel
{

	LayerStack::LayerStack()
	{
		

	}

	LayerStack::~LayerStack()
	{
		for(Layer* layer : m_layerstack  )
			delete layer ; //删除所有层 -> layerStack拥有layer所有权
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		HZ_PROFILE_FUNCTION();
		m_layerstack.emplace(m_layerstack.begin()+m_LayerInsertIndex,layer); 
		m_LayerInsertIndex++;	
		//m_LayerInsert++,原位置插入新layer  例子: 1 2 3 4 _<-Insert ,over1,over2, 
		//推送层 _ 覆盖层 ,覆盖层永远在推送层之上,m_layerInsert指推送层最后一个,在其前面进行推送层插入

	}

	void LayerStack::PushOverLayer(Layer* layer)
	{

		m_layerstack.emplace_back(layer); //覆盖层
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		//需要自己维护哪些层在推送啊....
		auto it = std::find(m_layerstack.begin(), m_layerstack.end(), layer);//弹出,但是layerstack失去其所有权,layer不会随之删除	
		if (it != m_layerstack.end()) 
		{
			m_layerstack.erase(it);
			m_LayerInsertIndex--;
		}

	}

	void LayerStack::PopOverLayer(Layer* layer)
	{
		auto it = std::find(m_layerstack.begin(), m_layerstack.end(), layer);
		if (it != m_layerstack.end()) // = .end() 应该是没找到的情况
		{
			m_layerstack.erase(it);
		}
	}

}
