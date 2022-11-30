#pragma once

#include "Event.h"	
#include "hzpch.h"
namespace Hazel
{

	class  WindowsResizeEvent :public Event
	{
	public:
		WindowsResizeEvent(unsigned int width,unsigned int heights)
			:m_width(width),m_height(heights) {};
		inline int GetWidth() const { return m_width; }
		inline int GetHeight() const { return m_height; }
		EVENT_CLASS_TYPE(WindowResize);
		EVENT_CLASS_CATEGORY(EventCategoryApplication); 

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowsResizeEvent: " <<m_width<<","<< m_height;
			return ss.str();
		}
	private:
		unsigned int m_width, m_height;

	};


	class  WindowsCloseEvent :public Event
	{
	public:
		WindowsCloseEvent() {};

		EVENT_CLASS_TYPE(WindowClose);
		EVENT_CLASS_CATEGORY(EventCategoryApplication); 

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowsCloseEvent" ;
			return ss.str();
		}
	};

	class  AppTickEvent :public Event
	{
	public:
		AppTickEvent() {};

		EVENT_CLASS_TYPE(AppTick);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "AppTickEvent";
			return ss.str();
		}
	};
	class  AppUpdateEvent:public Event
	{
	public:
		AppUpdateEvent() {};

		EVENT_CLASS_TYPE(AppUpdate);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);

// 		std::string ToString() const override
// 		{
// 			std::stringstream ss;
// 			ss << "AppUpdateEvent:";
// 			return ss.str();
// 		}
	};
	class  AppRenderEvent :public Event
	{
	public:
		AppRenderEvent() {};

		EVENT_CLASS_TYPE(AppRender);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "AppRenderEvent";
			return ss.str();
		}
	};
}