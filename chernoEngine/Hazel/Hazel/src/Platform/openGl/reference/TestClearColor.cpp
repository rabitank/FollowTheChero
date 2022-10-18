#include "TestClearColor.h"
#include "renderer.h"
#include "ImGui/imgui.h"

namespace test
{


	TestClearColor::TestClearColor()
		:m_Clearcolor{0.2f,0.3f,0.8f,1.0f}
	{

	}

	TestClearColor::~TestClearColor()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT)); //Çå³ýÑÕÉ«»º´æ
	}

	void TestClearColor::onUpdate(float deltaTime)
	{

	}

	void TestClearColor::onRender()
	{
		GLCall(glClearColor(m_Clearcolor[0], m_Clearcolor[1], m_Clearcolor[2], m_Clearcolor[3]));
		GLCall(glClear(GL_COLOR_BUFFER_BIT)); //Çå³ýÑÕÉ«»º´æ
	}

	void TestClearColor::onImGuiRender()
	{
		ImGui::ColorEdit4("clear color",m_Clearcolor);
	}


}