#pragma once

#include "Hazel/Core/Base.h"
#include <glm/glm.hpp>

namespace Hazel
{

	enum class FrameBufferTextureFormat
	{
		None = 0,

		RGB = 1, RGBA, RGBA16F, 
		
		RED_INTEGER,

		DEPTH24STENCIL8, DEPTH = DEPTH24STENCIL8,
	};


	enum class TextureWrap
	{
		Default =0
	};

	struct FrameBufferAttachmentSpecifcation
	{
		FrameBufferAttachmentSpecifcation() = default;
		FrameBufferAttachmentSpecifcation(FrameBufferTextureFormat format) 
			:Format(format)
		{};
		FrameBufferTextureFormat Format = FrameBufferTextureFormat::None;
		//TODO: use it to specific the texture wraptype
		TextureWrap warp;
	};

	struct FrameBufferSpecification //frameBuffer详细设定结构体 -
	{

		FrameBufferSpecification() = default;
		FrameBufferSpecification(uint32_t Width,uint32_t Height)
			:width(Width),height(Height) {};
		FrameBufferSpecification(const std::initializer_list<FrameBufferAttachmentSpecifcation>& attachments)
			:Attachments(attachments) {};

		uint32_t width, height;

		//what's this-> > if Frame need its attachment to have miniMap?
		uint32_t Samples = 1;

		//equal glBindBuffer(0); ...swapChain 在opengl中就是前后帧缓冲: backbuffer(后缓冲区)即帧缓冲/缓冲帧.. 如果只是不断显示与刷新一个buffer不需要"渲染通道"与交换链目标这样的概念/属性
		//这里还是为了跨平台准备..vulkan里swapchain是个显式提及的概念
		bool SwapChainTarget = false;
		std::vector<FrameBufferAttachmentSpecifcation> Attachments;

		glm::vec4 ClearColor = { 0.1f,0.1f,0.1f,1.f };
		//TODO:NoResize
		bool NoResize = false;

		std::vector<FrameBufferAttachmentSpecifcation>::iterator begin() { return Attachments.begin(); } //提供迭代器接口,方便遍历
		std::vector<FrameBufferAttachmentSpecifcation>::iterator end() { return Attachments.end(); }
		std::vector<FrameBufferAttachmentSpecifcation>::const_iterator begin() const { return Attachments.begin(); } //提供迭代器接口,方便遍历
		std::vector<FrameBufferAttachmentSpecifcation>::const_iterator end() const { return Attachments.end(); }//专门为const& 和const函数提供的 const迭代器

	}; 

	class  FrameBuffer
	{
	public:


		virtual ~FrameBuffer() = default; //需要有一个virtual析构 才会查看子类的析构并执行

		 //virtual FrameBufferSpecification& GetSpecification()const = 0;
		virtual const FrameBufferSpecification& GetSpecification() const = 0; //最好还是const....安全第一...

		virtual  uint32_t GetColorAttachMentRendererID(uint32_t index = 0) const = 0;
		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;


		virtual void ReSize(uint32_t width,uint32_t height) = 0;
		virtual int ReadPixel(uint32_t attachmentIndex,int x,int y) = 0;

		static Ref<FrameBuffer> Create(const FrameBufferSpecification& spec);
	private:

	};
}


