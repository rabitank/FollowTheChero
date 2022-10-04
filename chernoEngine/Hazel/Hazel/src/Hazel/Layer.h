#pragma once
#include "Hazel/Core.h"
#include "Hazel/Events/Event.h"

namespace Hazel
{
	class HAZEL_API Layer
		/// Layer ��һ��ջ���� , ������Ⱦ�ò�ͼ��,�����ò��¼�,����/��Ӧ/��ֹ�¼�����,�㽫�� LayerStack�ϸ��ݵ���˳��������Ⱦ������
	{
	public:
		Layer(const std::string & = "Layer"); //


		//�ȴ��̳�����д
		virtual ~Layer();

		virtual void OnAttach() {};// �� ��Layer �� �Ƶ� layerStack ��ʱʹ��
		virtual void OnDetach() {};// ��		 �� layerStack�� �� ʱ ʹ��
		virtual void OnUpdate() {};// �����ʱʹ�� ,ÿ֡����
		virtual void OnEvent(Event& event) {};

		inline const  std::string& Getname() { return m_Debugname; }

	protected:
		std::string m_Debugname; //��ʱ���������ִ�����ʵ��
	};

}

