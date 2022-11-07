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
<<<<<<< HEAD
		virtual ~OpenGlFrameBuffer(); //析构需要在其基类中存在....不然被视为基类时不会调用该类的析构函数/或者没有析构函数
=======
		virtual ~OpenGlFrameBuffer() {};
>>>>>>> 09c9513fa9700bc556c182756b0d40ca526de9c6

		void Invalidate(); //无效化...变成空白帧
		virtual void Bind() const override;	
		virtual void UnBind() const override;
		virtual const FrameBufferSpecification& GetSpecification() const override { return m_specification; }; //最好还是const....安全第一...
		virtual  uint32_t GetColorAttachMentRendererID() const { return m_colorAttachMent; }; //最好还是const....安全第一...

<<<<<<< HEAD
		virtual void ReSize(uint32_t width, uint32_t height) override;
=======
>>>>>>> 09c9513fa9700bc556c182756b0d40ca526de9c6

	private:
		FrameBufferSpecification m_specification;

<<<<<<< HEAD
		uint32_t m_rendererID =0;
		uint32_t m_colorAttachMent= 0;
		uint32_t m_depthAttachMent =0;
=======
		uint32_t m_rendererID;
		uint32_t m_colorAttachMent;
		uint32_t m_depthAttachMent;
>>>>>>> 09c9513fa9700bc556c182756b0d40ca526de9c6
	};

}

