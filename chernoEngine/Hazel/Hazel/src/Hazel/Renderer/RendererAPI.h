#pragma once

#include <glm/glm.hpp>
#include "VertexArray.h"

namespace Hazel
{
	class RendererAPI //this would not be visualable to user
	{
	public:
		enum class API
		{
			None = 0, OpenGl,

		};

	public:
		virtual ~RendererAPI() =default;
		virtual void SetClearColor(const glm::vec4& color)=0;
		virtual void Clear()=0; //¡ý
		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& VA , uint32_t indexCount = 0)=0; //¡ý
		virtual void Init()=0;
		virtual void SetViewPort(uint32_t x, uint32_t y,uint32_t width,uint32_t height)=0;

		static inline API GetAPI() { return s_API; }

	private:

		static API s_API;

	};

}