#include "Hazel.h"
//app 对引擎启动细节不感兴趣, 引擎入口(main)应该由引擎定义, app只需要通过实现来控制
//没有正确的程序入口点 是 dll没有更新,lib提供的函数入口与旧dll不对应引起的


class SandBox:public Hazel::Application
{
public:
	SandBox()
	{

	}
	~SandBox()
	{

	}
};

// a box about how the game work would be send/trans to engine's entry point

Hazel::Application* Hazel::CreateApplication()
{
	return new SandBox;

}