#include "hzpch.h"
#include "Renderer.h"
#include "Platform/openGl/OpenGlShader.h"
#include "Renderer2D.h"


namespace Hazel
{
	Renderer::SceneData* Renderer::m_sceneData = new Renderer::SceneData;

	void Renderer::BeginScene(OrthographicCamera& orC)
	{
		HZ_PROFILE_FUNCTION();

		m_sceneData->ViewProjectionMartix = orC.GetViewProjectionMat();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const Ref<Shader>& shader,const Ref<VertexArray>& vao, const glm::mat4& transformMat)
	{
		HZ_PROFILE_FUNCTION();

		shader->Bind();
		std::dynamic_pointer_cast<OpenGlShader>(shader)->UploadUniformMat4("u_ViewProjection",m_sceneData->ViewProjectionMartix);//我们需要为我们引擎的shader指定统一的uniform标准
		std::dynamic_pointer_cast<OpenGlShader>(shader)->UploadUniformMat4("u_Transform",transformMat);//我们需要为我们引擎的shader指定统一的uniform标准

		vao->Bind();
		RenderCommand::DrawIndexed(vao);
	}

	void Renderer::Init()
	{
		HZ_PROFILE_FUNCTION();

		RenderCommand::Init();
		Renderer2D::Init();

	}
	
	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewPort(0, 0, width, height);
	};


	

}
