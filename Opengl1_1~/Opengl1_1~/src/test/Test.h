#pragma once
#include <iostream>
#include <vector>
#include <functional> //for function
#include "ImGui/imgui.h"
namespace test
{
	class Test
	{
	public:
		Test() {};
		virtual ~Test() {}; //基类
		 
		virtual void onUpdate(float deltaTime) {}
		virtual void onRender() {}
		virtual void onImGuiRender() {}


	};

	class TestMenu :public Test
	{
	public:
		TestMenu(Test*& current);
		//在栈上,不需要
		void onImGuiRender()override;
		
		template<typename T>
		void RegisterTest(const std::string& lable)
		{
			std::cout << "Register a test:"<<lable<< std::endl;
			m_tests.push_back(std::make_pair(lable, [](){return new T();})); //堆上的,毕竟我们显式删除
		}
	private:
		Test*& m_currentTest;
		std::vector<std::pair<std::string, std::function<Test*()>>> m_tests;//testmenu 上的按钮按下会创建一个test ,这是个动作,需要lambda/函数指针


	};


}

