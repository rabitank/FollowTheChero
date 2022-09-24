#pragma once
#include "Test.h"
#include "renderer.h"
#include "VertexArray.h"
#include "BufferLayout.h"
#include "Texture.h"

namespace test
{
	class TestTexture:public Test
	{
	public:
		TestTexture();
		~TestTexture();

		void onUpdate(float deltaTime) override;
		void onRender() override;
		void onImGuiRender() override;

	private:
		glm::vec3 m_translationA;
		glm::vec3 m_translationB;
		std::unique_ptr<VertexArray> m_Vao;
		std::unique_ptr<IndexBuffer> m_IBO;
		std::unique_ptr<VertexBuffer> m_VBO;
		std::unique_ptr <Shader> m_shader;
		std::unique_ptr<Texture> m_Texture;
		glm::mat4 m_proj;
		glm::mat4 m_view;

			 
	};

}


