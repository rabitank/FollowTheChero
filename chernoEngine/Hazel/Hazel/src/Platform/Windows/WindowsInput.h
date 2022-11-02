#pragma once
#include "Hazel/Core/Input.h"

namespace Hazel
{
	class WindowsInput :public Input
	{
	public:
		WindowsInput() {};
		~WindowsInput() {};
	protected:
		virtual bool IsKeyPressedImple(int keycode) override;
		virtual bool IsMouseButtonPressedImple(int button);
		virtual float GetMouseXImple() override;
		virtual float GetMouseYImple() override;
		virtual std::pair<float, float>  GetMousePosImple() override;
		//依然有可能有其他实现
	};
}