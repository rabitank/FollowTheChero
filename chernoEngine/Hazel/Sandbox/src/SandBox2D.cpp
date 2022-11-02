#include "SandBox2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <chrono>






SandBox2D::SandBox2D()
	:Layer("SandBox2D"), m_cameraController(1280.f / 720.f, true)
{}

void SandBox2D::OnAttach()
{
	HZ_PROFILE_FUNCTION();

	m_boardTexture = Hazel::Texture2D::Create("Assets/Texture/99.png");
	m_quancifangTexture= Hazel::Texture2D::Create("Assets/Texture/quancifang.png");
	//m_boardTexture->Bind();

}

void SandBox2D::OnDetach()
{

}

void SandBox2D::OnImGuiRender()
{
	HZ_PROFILE_FUNCTION();


	ImGui::Begin("2DLayer Grid Control");
	ImGui::ColorEdit4("Grid Color", glm::value_ptr(m_flatColor));

	for(auto& result: m_result)
	{	


		char label[50];
		std::strcpy(label, "%.3fms ");
		std::strcat(label, result.Name); //cat 追加到结尾  "Name &.3fms"
		ImGui::Text(label,result.Time);
	
	}
	m_result.clear();

	ImGui::End();
}

void SandBox2D::OnUpdate(Hazel::Timestep deltaime)
{

	HZ_PROFILE_FUNCTION();
	{
		HZ_PROFILE_SCOPE("CameraController::OnUpdate");
		m_cameraController.OnUpdate(deltaime);
	}

	{
		HZ_PROFILE_SCOPE("Renderer Prepare ");
	Hazel::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
	Hazel::RenderCommand::Clear(); //↓	//why i can't see the layer before ? you clear it
	}

	static float rotation = 0.f;

	{

		rotation += deltaime;

		HZ_PROFILE_SCOPE("Render Draw");
		Hazel::Renderer2D::BeginScene(m_cameraController.GetCamera());//开始调度,设置场景参数 //提交场景的相机 ,注意 是"提交" , vp矩阵不是引用
		
		Hazel::Renderer2D::DrawQuad({ 1.f,0.2f }, { 0.3f,1.5f }, { 1.f,0.2f,0.4f,1.f });
		Hazel::Renderer2D::DrawQuad({ -1.f,-1.f,-0.1f }, { 1.f,1.f },{0.3f,0.5f,0.9f,1.0f});
		Hazel::Renderer2D::DrawQuad({ -5.f,-5.f,-0.2f }, { 10.f,10.f },m_boardTexture,10.f);
		//Hazel::Renderer2D::DrawQuad({ -0.5f,-0.5f,0.0f }, { 1.f,1.f },m_boardTexture,10.f);
		Hazel::Renderer2D::DrawQuad({ -1.f,1.f,0.1f }, { 1.f,1.f },m_quancifangTexture);
		Hazel::Renderer2D::DrawRotatedQuad({ 0.5f,0.2f }, { 2.f,2.f }, glm::degrees(rotation),m_quancifangTexture);
	//Hazel::Renderer2D::DrawQuad({ -1.f,-1.f,-0.1f }, { 10.f,10.f }, m_boardTexture, 10.0f, {0.3f,0.5f,0.9f,1.0f});
	}
	//Hazel::Renderer2D::Submit(m_flatShader, m_squareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));//提交shader,material,mesh,作为渲染内容
	Hazel::Renderer2D::EndScene();//一些清理工作


}

void SandBox2D::OnEvent(Hazel::Event& e)
{
	m_cameraController.OnEvent(e);

}
