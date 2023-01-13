#pragma once
#include "VertexArray.h"
#include "Shader.h"

#include <glm/glm.hpp>

#include "OrthographicCamera.h"
#include "Hazel/Renderer/Camera.h"

#include "Texture.h"
#include "SubTexture2D.h"

#include "Hazel/Renderer/EditorCamera.h"

#include "Hazel/Scene/Component.h"

namespace Hazel
{
	class Renderer2D
	{

	public:

		static void  BeginScene(const Camera& Camera, glm::mat4 viewMat=glm::mat4(1.f) );
		static void  BeginScene(const OrthographicCamera& Camera);//TODO:remove
		static void  BeginScene(const EditorCamera& Camera);//TODO:remove
		static void  EndScene();
		static void  ShutDown();
		static void Init();


		//Draw API
		static void Flush(); //refrash our va

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size,const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size,const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.f,const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.f,const glm::vec4& tintColor = glm::vec4(1.0f));

		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size,float rotation, const glm::vec4& color); //rotation should be radians
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size,float rotation, const glm::vec4& color);//rotation should be radians
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size,float rotation, const Ref<Texture2D>& texture, float tilingFactor = 1.f, const glm::vec4& tintColor = glm::vec4(1.0f));//rotation should be radians
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size,float rotation, const Ref<Texture2D>& texture, float tilingFactor = 1.f, const glm::vec4& tintColor = glm::vec4(1.0f));//rotation should be radians

		//Scene use them
		static void DrawQuad(const glm::mat4& transformat ,const glm::vec4& color,int entitID =-1);
		static void DrawQuad(const glm::mat4& transformat , const Ref<Texture2D>& texture, float tilingFactor = 1.f, const glm::vec4& tintColor = glm::vec4(1.0f), int entitID = -1);


		static void DrawLine();

		static void DrawCircle(const glm::mat4& transformat ,const glm::vec4& color,float thichness =1.f,float fade= 0.005f, int entitID =-1);


		//引入component , 我觉得不是很好
		static void DrawSprite(const glm::mat4& transformat, SpriteRendererComponent& src ,int entitID =-1);

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
		static void StartBatch();
		static void NextBatch();


	};

}

