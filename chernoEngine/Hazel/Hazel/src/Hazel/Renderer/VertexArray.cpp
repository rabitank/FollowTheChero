#include "hzpch.h"
#include "VertexArray.h"
#include "Platform/openGl/OpenGlVertexArray.h"
#include "Renderer.h"

namespace Hazel
{
	Ref<VertexArray> VertexArray::Create()
	{


		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGl:
			return std::make_shared<OpenGlVertexArray>(); //不是 public 继承方式甚至不允许转换到基类...是因为这样可以访问到基类吗?
		case RendererAPI::API::None:
			HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		}
		HZ_CORE_ASSERT(false, "Unkonw RendererAPI ,VertexBuffer::Create filed!")
			return nullptr;

	}

}
