#pragma once

#include "Hazel/Core/Base.h"

#include "hzpch.h"



//now it is block .
//when something happened ,for delivery of the message,
//would stop other code
namespace Hazel
{
	enum class  EventType //enum class 为枚举提供了枚举类型作用域
	{
		None = 0,
		WindowClose,WindowResize,WindowFocus,WindowLostFocus,WindowMoved,
		AppTick,AppUpdate,AppRender,
		KeyPressed,KeyReleased,KeyTyped,
		MouseButtonPressed,MouseButtonReleased,MouseMoved,MouseScrolled
	};

	enum HAZEL_API EventCategory //过滤器,互相将使用位与运算判断
	{
		None = 0,
		EventCategoryApplication = BIT(0),//1
		EventCategoryInput = BIT(1),//10
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4)


	};

	//宏创建这些零碎的函数

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType(){return EventType::##type;}\
								virtual EventType GetEventType() const override {return GetStaticType();}\
								virtual const char* GetName()const override{ return #type;}

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override{return category;}\

	class Event
	{
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const=0;
		virtual const char* GetName() const =0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); };

		bool Handled = false;

		inline bool IsInCategory(EventCategory eventcategory)
		{
			return GetCategoryFlags() & eventcategory; //位与运算
		}

		

	};

	class  EventDispatcher //调度器
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>; //std::function 函数对象类 封装一个bool返回,(T&)形参的函数/lambda/伪函数,这里使用了模板
	private:
		Event& m_event;
	
	public:
		EventDispatcher(Event& event)
			:m_event(event) {};//调度器收到事件
		template <typename T>
		bool DisPatch(EventFn<T> func)
		{
			if (m_event.GetEventType() == T::GetStaticType())
			{
				m_event.Handled = func(*(T*)&m_event); //调度器将会用事件调度 同类事件类的函数, 即 event 调度 对应event的动作/函数,这个event解决了吗,是否继续传递?由func决定
					return true;
			}
			return false;//调度失败
		};

	};

	inline std::ostream& operator<<(std::ostream& os, const Event& a)
	{
		return os << a.ToString(); //tostring自动化
	}

}
