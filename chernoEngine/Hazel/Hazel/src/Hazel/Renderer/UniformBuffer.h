#pragma once
#include "hzpch.h"

namespace Hazel
{
	//TODO: uniforBuffer for More complex Uniform And shader
	class UniformBuffer
	{
	public:
		static  Ref<UniformBuffer> Create(uint32_t size);

		virtual void SetData(const void* data, uint32_t size) = 0;
	};


}

