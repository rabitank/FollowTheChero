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
		virtual void DrawIndexed(const Ref<VertexArray>& VA); //¡ý


	};


}

