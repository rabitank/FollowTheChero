#include <iostream>
#include<GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.h"


#include "renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h" //记住,""是在同级找,再深入找,再在扩展库找
#include "VertexArray.h"
#include "BufferLayout.h"
#include "Texture.h"


#include "glm/ext.hpp"//for math
#include "glm/gtc/matrix_transform.hpp"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw_gl3.h"
#include "test/TestClearColor.h"



int main(void)
{


	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;
	//设置使用opengl版本3.3 , 使用核心/兼容文件
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//core不提供默认顶点数组,你必须显式创建一个顶点数组
	//opengl希望你用vao

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(960, 640, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);   //opengl 上下文到此行完成,你可以glewInit()了	
	glfwSwapInterval(1);//垂直同步(稳定帧率)

	GLenum err = glewInit();
	if (GLEW_OK != err)
		LOG("glewInit failed");
	LOG(glGetString(GL_VERSION));//打印版本  , 没有GPU后缀就是核显
	//VRAM 显存 缓冲区所在 ,现代opengl使用 并主要是对显卡编程
	//->shader
	//openGL可以理解为一种状态机 , *是上下文相关的*
	//指定 哪个buffer
	// 使用何种shader
	// 绘制什么
	// 画在哪
	//一一说明,然后Draw()!





	glm::vec3 translationA(200, 200, 0);
	glm::vec3 translationB(400, 200, 0);
	{

		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); //设置源数据alpha就是实际alpha,屏幕dest已画的为 1-src_alpha
		GLCall(glEnable(GL_BLEND));//开启blend


		renderer render;

		ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(window, true);
		ImGui::StyleColorsDark();

		test::Test* currentTest=nullptr;
		test::TestMenu* testMenu  =new test::TestMenu(currentTest);
		currentTest = testMenu;//当前测试是:menu
		testMenu->RegisterTest<test::TestClearColor>("Clear Color");//向menu中添加可选测试,  tip: 能转的是 const char* 到 const std::string

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			render.clear();

			ImGui_ImplGlfwGL3_NewFrame();
			if (currentTest)
			{
				currentTest->onUpdate(0.0f);
				currentTest->onRender();
				ImGui::Begin("Test"); //开启ImGui的一个线程 , 需要用End()返回
				
				if (currentTest != testMenu && ImGui::Button("<-")) //特性,当前一条件已经可以得出结果时,后面一条语句不会被执行(画出Button)
				{
					delete currentTest; //删除指向堆,栈上的指针还在
					currentTest = testMenu;
				}
				currentTest->onImGuiRender();
				ImGui::End();
			}

			//gl状态机已经绑定了一块有三角形顶点的内存,它知道画哪个
			//glDrawArrays(GL_TRIANGLES,0,6);

			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
		
		delete currentTest; //栈只会删除指针,这造成内存泄漏
		if(currentTest != testMenu)
			delete testMenu;

	}


	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	//gl的上下文到此结束,然鹅有些对象在栈退出时删除,调用需要上下文包含的析构函数,这会导致错误,以至不能自动退出程序
	return 0;
}

