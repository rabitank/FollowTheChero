#pragma once
#include "Hazel/Renderer/UniformBuffer.h"
namespace Hazel
{
	class OpenGlUniformBuffer:public UniformBuffer
	{
	public:
		OpenGlUniformBuffer();
		virtual void SetData(const void* data, uint32_t size) override;

	};

}

