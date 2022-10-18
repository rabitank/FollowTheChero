#pragma once

#include "Hazel/Renderer/Buffer.h"

namespace Hazel

{
	class OpenGlVertexBuffer : public VertexBuffer
	{

	public:
		OpenGlVertexBuffer(float* vertics, uint32_t size);
		virtual ~OpenGlVertexBuffer() ;

		virtual void Bind()const  override ;
		virtual void UnBind()const  override ;

		virtual void SetLayout(const BufferLayout& layout) override
		{
			m_bufferLayout = layout;
			//如果 layout 是const& 那么迭代器接口 begin也将需要 begin()const声明,返回const iterator,但是这是不允许的(结构上不行),导致我们没有对应const&的迭代器函数实例,我们只能以非const方法使用自己的Bufferlayout
			//但是! stl弥补了这一点,你可以使用 const_iterator 返回值来完成 begin() const版本实例
			//ApplyLayout();
		};
		inline virtual const BufferLayout& GetLayout() const override
		{
			return m_bufferLayout;
		};

	private:
		//void ApplyLayout() const ;//Apply也不能 const,需要layout使用同样的Begin() const实例
		uint32_t m_rendererID;
		BufferLayout m_bufferLayout;
	};

	class OpenGlIndexBuffer : public IndexBuffer
	{

	public:
		OpenGlIndexBuffer(uint32_t* indie, uint32_t size);
		virtual ~OpenGlIndexBuffer() ;

		virtual void Bind() const override;
		virtual void UnBind()const  override;
		virtual uint32_t GetCount() const override { return m_count; };

	private:

		uint32_t m_rendererID;
		uint32_t m_count;


	};

}