#include "hzpch.h"

#include "Buffer.h"
#include "Platform/openGl/OpenGlBuffer.h"
#include "Renderer.h"

namespace Hazel
{



	Ref<VertexBuffer>  VertexBuffer::Create(float* vertics, uint32_t size)
	{
		//which api
		switch (Renderer::GetAPI() )
		{
		case RendererAPI::API::OpenGl:
			return CreateRef<OpenGlVertexBuffer>(vertics,size); //不是 public 继承方式甚至不允许转换到基类...是因为这样可以访问到基类吗?
		case RendererAPI::API::None:
			HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		}
		HZ_CORE_ASSERT(false,"Unkonw RendererAPI ,VertexBuffer::Create filed!")
		return nullptr;
	}

	 Ref<VertexBuffer>  VertexBuffer::Create(uint32_t size)
	 {
		 //which api
		 switch (Renderer::GetAPI())
		 {
		 case RendererAPI::API::OpenGl:
			 return CreateRef<OpenGlVertexBuffer>(size); //不是 public 继承方式甚至不允许转换到基类...是因为这样可以访问到基类吗?
		 case RendererAPI::API::None:
			 HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			 return nullptr;
		 }
		 HZ_CORE_ASSERT(false, "Unkonw RendererAPI ,VertexBuffer::Create filed!")
			 return nullptr;
	 }

	 //Current support size_t bit index buffers
	 Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, size_t count)
	{
	
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGl:
			return CreateRef<OpenGlIndexBuffer>(indices, count);
		case RendererAPI::API::None:
			HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		}

		
		HZ_CORE_ASSERT(false,"Unkonw RendererAPI,IndexBuffer::Create filed!")
		return nullptr;

	};



	 BufferLayout::~BufferLayout()
	 {

	 }

}