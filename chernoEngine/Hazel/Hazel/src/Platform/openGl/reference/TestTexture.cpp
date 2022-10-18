#include "TestTexture.h"
#include "ImGui/imgui.h"



#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Texture.h"
namespace test
{




	TestTexture::TestTexture()
		:m_translationA(200, 200, 0), m_translationB(400, 200, 0),
		m_proj(glm::ortho(0.0f, 960.0f, 0.0f, 640.0f, -1.0f, 1.0f)),
		m_view(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))

	{
		float Position[] = //position + uv
		{
		-50.0f,-50.0f,0.0f,0.0f,
		50.0f ,-50.0f,1.0f,0.0f,
		50.0f ,50.0f ,1.0f,1.0f,
		-50.0f,50.0f ,0.0f,1.0f
		};
		//除了数据缓冲区,我们还需要索引缓冲区	
		//省重复内存 0,1,2,2,3,0
		unsigned int indices[] =
		{
			0,1,2,
			2,3,0
		};

		//关于opengl的函数  , 你可以去 docs.gl 网站查询详细信息	
		m_Vao = std::make_unique<VertexArray>();
		m_IBO = std::make_unique<IndexBuffer>(indices, 6);
		m_shader = std::make_unique<Shader>("res/shaders/basic.shader");
		m_VBO = std::make_unique<VertexBuffer>(Position,4*4*sizeof(float));
		//VertexBuffer vb(Position, 4 * 4 * sizeof(float));
		//这里的VertexBuffer 在cpu 与 Gpu之间仍是 映射关系. cpu的vb被删, Gpu分配的空间同样被删,这时vb的析构函数会试着再删一次GPU上分配的vb,这导致了崩溃
		//所以我们需要转而在cpu上持有维护vb

		BufferLayout Layout;
		Layout.Push<float>(2);
		Layout.Push<float>(2);
		m_Vao->AddBuffer(*m_VBO, Layout);




		//m模型 v视图 p投影 矩阵
		//投影(正交/透视) //模型(模型变换) //视图(模拟相机,位置与方向,对模型进行逆变换)
		 //lrbt nf:目标框/屏幕框范围
		//返回的 proj*(l,b,n,1)T = (-1~1, -1~1, -1~1 1)T
		//a view : glm 的矩阵是以列向量排布的
// 		for (int i =0; i <proj.length();i++)
// 		{
// 			std::cout<< proj[i].x<<" "<<proj[i].y << " " << proj[i].z << " " << proj[i].w << std::endl;
// 		}
		//glm::mat4 modle = glm::translate(glm::mat4(1.0f),glm::vec3(300,200,0)); //平移
		
		//glm::mat4 mvp = proj * view * modle;


		
		m_shader->Bind();

		//不要把图片通过vs拖进来!!!!!vs会骗人的!!!!!
		//但是 资源管理器 不会 ^ . ^
		m_Texture= std::make_unique<Texture>("res/tex/theChernoLogo.png");
		m_Texture->Bind();
		m_shader->SetUniform1i("u_Texture", 0);//使用纹理插槽0
		m_shader->SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);


		//解绑
		m_Vao->Unbind();
		m_VBO->Unbind();
		m_IBO->Unbind();
		m_shader->Unbind();
	}

	TestTexture::~TestTexture()
	{	
		
	}

	void TestTexture::onUpdate(float deltaTime)
	{

	}

	void TestTexture::onRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT)); //清除颜色缓存
		renderer m_render;

		{
			glm::mat4 modle = glm::translate(glm::mat4(1.0f), m_translationA); //平移
			glm::mat4 mvp = m_proj * m_view * modle;
			m_shader->Bind();
			m_shader->SetUniformMat4f("u_MVP", mvp);
			m_render.Drawer(*m_Vao,*m_IBO,*m_shader);
		}
		{
			glm::mat4 modle = glm::translate(glm::mat4(1.0f),m_translationB); //平移
			glm::mat4 mvp = m_proj * m_view * modle;
			m_shader->Bind();
			m_shader->SetUniformMat4f("u_MVP", mvp);
			m_render.Drawer(*m_Vao, *m_IBO,*m_shader); //批渲染(渲染多次)

		}
	}

	void TestTexture::onImGuiRender()
	{
		ImGui::SliderFloat3("Translation A", &m_translationA.x, 0.0f, 960.0f); //一个设置滑块,传值到translation // Edit 3 float using a slider from 0.0f to 960.0f    
		ImGui::SliderFloat3("Translation B", &m_translationB.x, 0.0f, 960.0f); //一个设置滑块,传值到translation  
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

}