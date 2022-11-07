#include "hzpch.h"
#include "FrameBuffer.h"

#include "Renderer.h"
#include "Platform/openGl/OpenGLFrameBuffer.h"


namespace Hazel
{



	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{			
			case RendererAPI::API::OpenGl: return CreateRef<OpenGlFrameBuffer>(spec); 
			case RendererAPI::API::None: HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
	
		}
		HZ_CORE_ASSERT(false, "Unkonw RendererAPI,FrameBuffer::Create filed!")
			return nullptr;
	}



}
