#include "TestBatchRender.h"

#include "Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test
{
	TestBatchRender::TestBatchRender()
		:m_Proj(glm::ortho(0.0f, 640.0f, 0.0f, 480.0f, -1.0f, 1.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0))),
		m_Translation(glm::vec3(0, 0, 0))
	{

		const size_t MaxQuadCount = 100;
		const size_t MaxVertexCount = MaxQuadCount * 4;
		const size_t MaxIndexCount = MaxQuadCount * 6;

		float positions[] = {
			100.0f, 100.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
			200.0f, 100.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			200.0f, 200.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
			100.0f, 200.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,

			300.0f, 100.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			400.0f, 100.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
			400.0f, 200.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			300.0f, 200.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,

		};

		uint32_t indices[MaxIndexCount];
		uint32_t offset=0;
		for (size_t i = 0; i < MaxIndexCount; i += 6)
		{
			
			indices[i + 0] = 0 + offset;
			indices[i + 1] = 1 + offset;
			indices[i + 2] = 2 + offset;
			indices[i + 3] = 2 + offset;
			indices[i + 4] = 3 + offset;
			indices[i + 5] = 0 + offset;
			offset += 4;
		}

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		m_VAO = std::make_unique<VertexArray>();

		BufferLayout layout;
		layout.Push<float>(4); // 坐标 x, y, z, w  w 齐次坐标, 对xyz进行缩放
		layout.Push<float>(4); // 颜色数据
		layout.Push<float>(2); // 纹理坐标
		layout.Push<float>(1); // 纹理插槽

		m_VertexBuffer = std::make_unique<VertexBuffer>(positions,  MaxVertexCount*11* sizeof(float));
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, MaxIndexCount);

		m_VAO->AddBuffer(*m_VertexBuffer, layout);

		m_Shader = std::make_unique<Shader>("res/shaders/Batch.shader");
		m_Shader->Bind();

		m_Texture[0] = std::make_unique<Texture>("res/tex/theChernoLogo.png");
		m_Texture[1] = std::make_unique<Texture>("res/tex/yuan.png");
		for (size_t i = 0; i < 2; i++)
		{
			m_Texture[i]->Bind(i);
		}
		int samplers[2] = { 0, 1 };
		m_Shader->SetUniform1iv("u_Textures", 2, samplers);
	}

	TestBatchRender::~TestBatchRender()
	{
	}

	void TestBatchRender::onUpdate(float deltaTime)
	{
	}

	void TestBatchRender::onRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		renderer render ; // ÿ֡���renderer��Ҫ��һ����
		glm::mat4 mvp = m_Proj * m_View;

		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_MVP", mvp);

		render.Drawer(*m_VAO, *m_IndexBuffer, *m_Shader);
	}

	void TestBatchRender::onImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}