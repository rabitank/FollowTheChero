#pragma once
#include "Hazel/Renderer/RendererAPI.h"

namespace Hazel
{
	class OpenGlRendererAPI:public RendererAPI
	{
	public:
		OpenGlRendererAPI() {};

		virtual void Init() override ;

		virtual void SetClearColor(const glm::vec4& color) ;

		virtual void Clear() ; //¡ý
		virtual void DrawIndexed(const Ref<VertexArray>& VA, uint32_t indexCount  = 0) override; //¡ý
		virtual void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

	};


}

