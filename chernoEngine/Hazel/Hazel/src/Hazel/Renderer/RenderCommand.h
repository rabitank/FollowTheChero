#pragma once

#include "RendererAPI.h"


namespace Hazel
{
	class  RenderCommand
	{
	public:
		virtual ~RenderCommand() {};

		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawIndexed(vertexArray);
		};

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