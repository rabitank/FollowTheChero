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

		virtual void Clear() ; //↓
		virtual void DrawIndexed(const Ref<VertexArray>& VA, uint32_t indexCount  = 0) override; //↓
		virtual void DrawLines(const Ref<VertexArray>& VA, uint32_t vertexCount) override;
		virtual void SetLineWidth(float width) override; // gl对画线粗细的设置是全局的...

		virtual void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
	};


}

