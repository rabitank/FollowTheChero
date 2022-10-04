
#include "Event.h"

#include "hzpch.h"
namespace Hazel
{

	class HAZEL_API KeyEvent:public Event  //类内写的函数定义会被视为inline
	{
	public:
		
		inline int GetKeyCode() const{return m_keycode;}
		
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard ) // | 或运算赋
	protected://允许子类访问
		KeyEvent(int Event):m_keycode(Event){} //是内联的
		int m_keycode;
	};

	class HAZEL_API KeyPressedEvent :public KeyEvent //按下按键 在按一次和长按间需要缓冲时间把这两件事区分开
	{
		

	public:
		inline int GetRepeatcount() const { return m_repeatCount; }

		KeyPressedEvent(int keycode, int repeatc)
			:KeyEvent(keycode), m_repeatCount(repeatc) {}; //调用基类构造函数
		
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_keycode << "(" << m_repeatCount << "repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed);
	private:
		int m_repeatCount;
	};

	class HAZEL_API KeyReleasedEvent :public KeyEvent //按下按键 在按一次和长按间需要缓冲时间把这两件事区分开
	{


	public:

		KeyReleasedEvent(int keycode)
			:KeyEvent(keycode) {}; //调用基类构造函数

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_keycode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased);

	};


}