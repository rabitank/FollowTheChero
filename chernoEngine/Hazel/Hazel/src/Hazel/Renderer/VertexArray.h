#pragma once
#include "Hazel/Renderer/Buffer.h"
#include "hzpch.h"
namespace Hazel
{
	class VertexArray
	{
		
	public:
		virtual ~VertexArray() {};
		static VertexArray* Create();


		virtual void Bind()=0;
		virtual void UnBind()=0;
		
		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexbuffer ) = 0;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers()const = 0;
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;

	};

}

