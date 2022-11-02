#pragma once
#include "Hazel.h"
#include "ImGui/imgui.h"
class SandBox2D:public Hazel::Layer
{
public:



	SandBox2D();

	virtual void OnAttach()override;
	virtual void OnDetach()override;
	virtual void OnImGuiRender() override;
	void OnUpdate(Hazel::Timestep deltaime) override;
	void OnEvent(Hazel::Event& e) override;//调度的解决判断在application 的Onevent ,不用管

private:

	struct  ProfileResult
	{
		const char* Name;
		float Time;
	};

	Hazel::Ref<Hazel::VertexArray> m_squareVA; //for test

	Hazel::Ref<Hazel::Texture2D> m_boardTexture,m_quancifangTexture;

	Hazel::OrthographicCameraController m_cameraController; //cherno把position提出来了?? 
	glm::vec4 m_flatColor = { 0.2,0.3f,0.8f,0.4f};


	std::vector<ProfileResult> m_result;

};

