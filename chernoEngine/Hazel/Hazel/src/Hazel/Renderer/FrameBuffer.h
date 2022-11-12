#pragma once

#include "Hazel/Core/Base.h"

namespace Hazel
{

	struct FrameBufferSpecification //frameBuffer详细设定结构体
	{

		uint32_t width, height;
		//FrameBufferFormat Format =  //deffault set
		uint32_t samples = 1;

		bool SwapChainTarget = false;
		//equal glBindBuffer(0); ...swapChain 在opengl中就是前后帧缓冲: backbuffer(后缓冲区)即帧缓冲/缓冲帧.. 如果只是不断显示与刷新一个buffer不需要"渲染通道"与交换链目标这样的概念/属性
		//这里还是为了跨平台准备..vulkan里swapchain是个显式提及的概念




	}; 

	class  FrameBuffer
	{
	public:


		virtual ~FrameBuffer() = default; //需要有一个virtual析构 才会查看子类的析构并执行

		 //virtual FrameBufferSpecification& GetSpecification()const = 0;
		virtual const FrameBufferSpecification& GetSpecification() const = 0; //最好还是const....安全第一...
		virtual  uint32_t GetColorAttachMentRendererID() const = 0;
		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;


		virtual void ReSize(uint32_t width,uint32_t height) = 0;


		static Ref<FrameBuffer> Create(const FrameBufferSpecification& spec);
	private:

	};
}


