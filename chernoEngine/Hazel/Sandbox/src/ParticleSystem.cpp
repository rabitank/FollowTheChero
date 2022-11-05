#include "ParticleSystem.h"


#include "Random.h"

#include <glm/gtc/constants.hpp>
#include <glm/gtx/compatibility.hpp>

#define GLM_ENABLE_EXPERIPENTAL


ParticleSystem::ParticleSystem(uint32_t maxParticlePool)
	:m_poolIndex(maxParticlePool-1)
{
	m_ParticlePool.resize(maxParticlePool); //不...一共 maxParticlepool 个粒子由index遍历...整个画面最多只能存在maxParticlepool个粒子
}


void ParticleSystem::OnUpdate(Hazel::Timestep ts) //更新需要渲染的粒子
{
	for (auto& particle : m_ParticlePool)
	{
		if (!particle.Active)
		{
			continue;
		}
		if (particle.LifeRemaining <= 0.f)
		{	
			particle.Active = false;
			continue;
		}

		particle.LifeRemaining -= ts;
		particle.Position += particle.Velocity * (float)ts;
		particle.Rotation += 0.01f * ts;

	}
}

void ParticleSystem::OnRender(Hazel::OrthographicCamera camera) //绘制pool存活的粒子
{

	//m_particleShader->SetMat4()// noop , it should be done at Init;q

	Hazel::Renderer2D::BeginScene(camera);
	for (auto& particle : m_ParticlePool)
	{

		if(!particle.Active)
			continue;

		float life = particle.LifeRemaining / particle.LifeTime;
		glm::vec4 color = glm::lerp(particle.ColorBegin,particle.ColorEnd, life);
		float size = glm::lerp(particle.SizeBegin,particle.SizeEnd, life);
		

		Hazel::Renderer2D::DrawRotatedQuad(glm::vec3(particle.Position,0.2f), { size,size }, particle.Rotation, color);

	}

	Hazel::Renderer2D::EndScene();

}

void ParticleSystem::Emit(const ParticleProps& particleprops)//激活并配置粒子
{
	Particle& particle = m_ParticlePool[m_poolIndex];
	particle.Active = true;
	particle.Position = particleprops.Position;
	particle.Rotation = Random::Float() * 2.f * glm::pi<float>(); //Random return 0-1

	//Velocity
	particle.Velocity = particleprops.Velocity;
	particle.Velocity.x += particleprops.VelocityVariation.x * (Random::Float() - 0.5f);
	particle.Velocity.y += particleprops.VelocityVariation.y * (Random::Float() - 0.5f);

	//color
	particle.ColorBegin = particleprops.colorBegin;
	particle.ColorEnd= particleprops.colorEnd;


	particle.LifeTime = particleprops.LifeTiem;
	particle.LifeRemaining = particleprops.LifeTiem;
	particle.SizeBegin = particleprops.SizeBeging;
	particle.SizeEnd= particleprops.SizeEnd;

	m_poolIndex = --m_poolIndex % m_ParticlePool.size();

}
