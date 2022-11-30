#pragma once
#include "Hazel/Renderer/UniformBuffer.h"
namespace Hazel
{
	class OpenGlUniformBuffer:public UniformBuffer
	{
	public:
		OpenGlUniformBuffer(uint32_t size, uint32_t binding);
		virtual ~OpenGlUniformBuffer() ;

		virtual void SetData(const void* data, uint32_t size, uint32_t offset=0) override;
	private:
		uint32_t m_rendererID = 0;

	};

}

