#pragma once
#include "Hazel/Renderer/VertexArray.h"
#include "Glad/glad.h"

namespace Hazel
{
	class OpenGlVertexArray:public VertexArray
	{
	public:
		OpenGlVertexArray();
		~OpenGlVertexArray();
		void virtual Bind() override;
		void virtual UnBind() override;

		void AddVertexBuffer(const Ref<VertexBuffer>& vertexbuffer)override;
		void SetIndexBuffer(const Ref<IndexBuffer>& indexbuffer)override;

		inline virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override { return m_vertexBuffers; };
		inline virtual const Ref<IndexBuffer>& GetIndexBuffer()const override { return m_indexBuffer; };



	private:
		uint32_t m_rendererID;
		std::vector<Ref<VertexBuffer>> m_vertexBuffers; //many vertexBuffer use one VertexArray?? a kind of runmode?
		Ref<IndexBuffer	>m_indexBuffer;


	};

}

