#include "TestTexture.h"
#include "renderer.h"
#include "ImGui/imgui.h"

#include "VertexArray.h"
#include "BufferLayout.h"
#include "glm/gtc/matrix_transform.hpp"
namespace test
{




	TestTexture::TestTexture()
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
		VertexArray va;
		VertexBuffer vb(Position, 4 * 4 * sizeof(float));

		BufferLayout Layout;
		Layout.Push<float>(2);
		Layout.Push<float>(2);
		va.AddBuffer(vb, Layout);

		IndexBuffer ib(indices, 6);


		//m模型 v视图 p投影 矩阵
		//投影(正交/透视) //模型(模型变换) //视图(模拟相机,位置与方向,对模型进行逆变换)
		glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 640.0f, -1.0f, 1.0f); //lrbt nf:目标框/屏幕框范围
		//返回的 proj*(l,b,n,1)T = (-1~1, -1~1, -1~1 1)T
		//a view : glm 的矩阵是以列向量排布的
// 		for (int i =0; i <proj.length();i++)
// 		{
// 			std::cout<< proj[i].x<<" "<<proj[i].y << " " << proj[i].z << " " << proj[i].w << std::endl;
// 		}
		//glm::mat4 modle = glm::translate(glm::mat4(1.0f),glm::vec3(300,200,0)); //平移
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		//glm::mat4 mvp = proj * view * modle;

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
	}

	void TestTexture::onImGuiRender()
	{

	}

}