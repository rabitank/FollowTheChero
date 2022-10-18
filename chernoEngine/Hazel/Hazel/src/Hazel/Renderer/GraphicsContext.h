#pragma once

namespace Hazel
{

	class GraphicsContext //纯抽象类,实现在platform里
	{
	public:
		virtual void Init()=0;
		virtual void SwapBuffers() =0;

	};


}

