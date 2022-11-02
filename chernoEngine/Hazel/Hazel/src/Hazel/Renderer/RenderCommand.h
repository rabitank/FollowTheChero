#pragma once

#include "RendererAPI.h"


namespace Hazel
{
	class  RenderCommand
	{
	public:
		virtual ~RenderCommand() {};

		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0)
		{
			s_RendererAPI->DrawIndexed(vertexArray, indexCount);
		};
		inline static void SetViewPort(uint32_t x, uint32_t y,uint32_t width, uint32_t height) { s_RendererAPI->SetViewPort(x,y,width, height); }

		inline static void Clear() { s_RendererAPI->Clear(); }
		inline static void SetClearColor(const glm::vec4& color) { s_RendererAPI->SetClearColor(color); }


		inline static void Init()
		{
			s_RendererAPI->Init();

		};
	private:
		static RendererAPI* s_RendererAPI;

	};


}