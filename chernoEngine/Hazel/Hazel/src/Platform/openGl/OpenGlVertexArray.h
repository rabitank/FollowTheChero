#pragma once
#include "Hazel/Renderer/VertexArray.h"

namespace Hazel
{
	class OpenGlVertexArray:public VertexArray
	{
	public:
		OpenGlVertexArray();
		~OpenGlVertexArray();
		void virtual Bind() override;
		void virtual UnBind() override;

		void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexbuffer)override;
		void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexbuffer)override;

		inline virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override { return m_vertexBuffers; };
		inline virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer()const override { return m_indexBuffer; };



	private:
		uint32_t m_rendererID;
		std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers; //many vertexBuffer use one VertexArray?? a kind of runmode?
		std::shared_ptr<IndexBuffer	>m_indexBuffer;


	};

}

