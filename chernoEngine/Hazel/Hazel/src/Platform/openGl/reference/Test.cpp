#include "Test.h"

namespace test
{
	TestMenu::TestMenu(Test*& current)//如果是 Test* 就还是新建一个栈上的指针传值而已!!!,
		:m_currentTest(current)
	{

	}
	void TestMenu::onImGuiRender()
	{
		for (auto& atest : m_tests)
		{
			if (ImGui::Button(atest.first.c_str()))
			{
				m_currentTest = atest.second();//执信创建test动作
			}
			
		}
		
	}
}