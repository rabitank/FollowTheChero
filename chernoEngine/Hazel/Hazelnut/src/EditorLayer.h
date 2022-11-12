#pragma once
#include "Hazel.h"
#include "ImGui/imgui.h"



namespace Hazel
{

	class EditorLayer :public Layer
	{
	public:



		EditorLayer();

		virtual void OnAttach()override;
		virtual void OnDetach()override;
		virtual void OnImGuiRender() override;
		void OnUpdate(Timestep deltaime) override;
		void OnEvent(Event& e) override;//调度的解决判断在application 的Onevent ,不用管

	private:

		struct  ProfileResult
		{
			const char* Name;
			float Time;
		};

		//old ref
		Ref<VertexArray> m_squareVA; //for 
		Ref<Texture2D> m_boardTexture, m_quancifangTexture;
		Ref<Texture2D> m_spriteSheet;
		Ref<SubTexture2D> m_spriteHair, m_spriteTree;
		Ref<FrameBuffer> m_frameBuffer;	

		Ref<Scene> m_activeScene;

		OrthographicCameraController m_cameraController; //cherno把position提出来了?? 
		
		glm::vec4 m_flatColor = { 0.2,0.3f,0.8f,0.4f };
		glm::vec2 m_ViewPortSize;


		//Paricle 
		ParticleSystem m_particleSystem;
		ParticleProps m_particle;


		//Entity: quad and Camera
		Entity m_quadEntity;
		Entity m_cameraEntity;
		Entity m_secondCamera;
		bool m_primaryCamera =false;



		std::unordered_map<char, Hazel::Ref<SubTexture2D>> m_TextureMap; //for char to texture
		uint32_t m_mapWidth, m_mapHeight;

		bool m_viewPortIsFocus =false, m_viewPortIsHover =false;

	};

}

