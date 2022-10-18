#include "hzpch.h"
#include "VertexArray.h"

#include "Hazel/Renderer/Renderer.h"

#include "Platform/openGl/OpenGlVertexArray.h"

namespace Hazel
{
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::s_rendererAPI)
		{
		case RendererAPI::OpenGl:
			return new OpenGlVertexArray(); //不是 public 继承方式甚至不允许转换到基类...是因为这样可以访问到基类吗?
		case RendererAPI::None:
			HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		}
		HZ_CORE_ASSERT(false, "Unkonw RendererAPI ,VertexBuffer::Create filed!")
			return nullptr;

	}

}
