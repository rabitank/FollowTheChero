#pragma once
#include "VertexArray.h"
#include "Shader.h"


#include <glm/glm.hpp>
#include "OrthographicCamera.h"
#include "Texture.h"
#include "SubTexture2D.h"

namespace Hazel
{
	class Renderer2D
	{
	public:
		static void  BeginScene(const OrthographicCamera& Camera);
		static void  EndScene();
		static void  ShutDown();
		static void Init();


		//Draw API
		static void Flush(); //refrash our va

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size,const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size,const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.f,const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.f,const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tilingFactor = 1.f,const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tilingFactor = 1.f,const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawLine();

		//Use Radiance
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size,float rotation, const glm::vec4& color); //rotation should be radians
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size,float rotation, const glm::vec4& color);//rotation should be radians
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size,float rotation, const Ref<Texture2D>& texture, float tilingFactor = 1.f, const glm::vec4& tintColor = glm::vec4(1.0f));//rotation should be radians
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size,float rotation, const Ref<Texture2D>& texture, float tilingFactor = 1.f, const glm::vec4& tintColor = glm::vec4(1.0f));//rotation should be radians
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size,float rotation, const Ref<SubTexture2D>& subTexture, float tilingFactor = 1.f, const glm::vec4& tintColor = glm::vec4(1.0f));//rotation should be radians
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size,float rotation, const Ref<SubTexture2D>& subTexture, float tilingFactor = 1.f, const glm::vec4& tintColor = glm::vec4(1.0f));//rotation should be radians

		//stats
		struct  Statistic //统计数据结构
		{
			uint32_t DrawCalls = 0;
			uint32_t quadCount = 0;
			uint32_t GetTotalVertexCount() { return quadCount * 4; };
			uint32_t GetTotalIndexCount() { return quadCount * 6; };
		};
		
		static void ResetStats();
		static Statistic& GetStats();


	private:

		static void FlushAndReset();


	};

}

