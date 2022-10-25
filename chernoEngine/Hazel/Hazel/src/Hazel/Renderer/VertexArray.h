#pragma once
#include "hzpch.h"
#include "Hazel/Renderer/Buffer.h"

namespace Hazel
{
	class VertexArray
	{
		
	public:
		virtual ~VertexArray() {};
		static Ref<VertexArray> Create();


		virtual void Bind()=0;
		virtual void UnBind()=0;
		
		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexbuffer ) = 0;
		virtual void SetIndexBuffer(const  Ref<IndexBuffer>& indexBuffer) = 0;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers()const = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;

	};

}

