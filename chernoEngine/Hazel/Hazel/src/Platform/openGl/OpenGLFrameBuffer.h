#pragma once
#include "Hazel/Renderer/FrameBuffer.h"

namespace Hazel
{
	/// <summary>
	///帧缓存的概念就是,把输出结果都组合一下渲染成一张图,有窗口那么大,实际显示的就是这张图,你可以手动设置它直接让显示器显示你想显示的,或者用这个承接渲染器渲染的东西(Bind,UnBind)
	/// </summary>
	class OpenGlFrameBuffer:public FrameBuffer
	{
	public:
		OpenGlFrameBuffer(const FrameBufferSpecification& spec);

		virtual ~OpenGlFrameBuffer(); //析构需要在其基类中存在....不然被视为基类时不会调用该类的析构函数/或者没有析构函数


		virtual int8_t ReadPixel(uint32_t attachmentIndex ,int x,int y) override;
		void Invalidate(); //无效化...变成空白帧
		virtual void Bind() const override;	
		virtual void UnBind() const override;
		virtual const FrameBufferSpecification& GetSpecification() const override { return m_specification; }; //最好还是const....安全第一...
		virtual  uint32_t GetColorAttachMentRendererID(uint32_t index =0) const { return m_colorAttachmentIDs[index]; }; //最好还是const....安全第一...
		virtual void ClearColorAttachment(uint32_t attachmentIndex, int value) override;


		virtual void ReSize(uint32_t width, uint32_t height) override;


	private:
		FrameBufferSpecification m_specification;


		uint32_t m_rendererID =0;

		//分开color与depth -> color最多有四个且需要计数??
		
		std::vector<FrameBufferAttachmentSpecifcation> m_colorAttachmentspecifications;
		FrameBufferAttachmentSpecifcation m_depthAttchmentSpecification ;

		std::vector<uint32_t> m_colorAttachmentIDs;
		uint32_t m_depthAttachmentID = 0;
	};

}

