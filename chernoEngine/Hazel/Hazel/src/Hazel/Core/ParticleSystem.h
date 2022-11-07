#pragma once

#include "Hazel/Renderer/Shader.h"
#include "Timestep.h"
#include "Hazel/OrthographicCameraController.h"
#include <glm/glm.hpp>



namespace Hazel

{
	struct ParticleProps
	{
		glm::vec2 Position;
		glm::vec2 Velocity, VelocityVariation; //速度，速度变化(a?);
		glm::vec4 colorBegin, colorEnd;

		float SizeBeging,SizeEnd,SizeVariation; //尺寸设定
		float LifeTiem = 1.0f;
	};


	class ParticleSystem
	{
	public:
		ParticleSystem(uint32_t maxParticlePool=1000);
		void OnUpdate(Timestep ts);
		void OnRender(OrthographicCamera camera);

		void Emit(const ParticleProps& particleprops); //发射?

	private:
		struct  Particle
		{
			glm::vec2 Position;
			glm::vec2 Velocity;
			glm::vec4 ColorBegin,ColorEnd;
		

			float Rotation = 0.f;
			float SizeBegin, SizeEnd;


			float LifeTime =1.0f;
			float LifeRemaining = 0.f;

			bool Active = false;
		};
		std::vector<Particle> m_ParticlePool;//all partical
		uint32_t m_poolIndex ;
		std::unique_ptr<Shader> m_particleShader;
		int m_particleShaderViewProj, m_particleShaderTransform, m_ParticleShaderColor;


	
	};

}