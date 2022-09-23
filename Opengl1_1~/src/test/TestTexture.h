#pragma once
#include "Test.h"

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
			 
	};

}


