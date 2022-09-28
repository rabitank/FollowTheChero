
#include "Event.h"

#include <sstream>
namespace Hazel
{

	class HAZEL_API WindowsResizeEvent :public Event
	{
	public:
		WindowsResizeEvent(unsigned int width,unsigned int heights)
			:m_width(width),m_height(heights) {};
		inline int GetWeight() const { return m_width; }
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


	class HAZEL_API WindowsCloseEvent :public Event
	{
	public:
		WindowsCloseEvent() {};

		EVENT_CLASS_TYPE(WindowClose);
		EVENT_CLASS_CATEGORY(EventCategoryApplication); 

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowsResizeEvent" ;
			return ss.str();
		}
	};

	class HAZEL_API AppTickEvent :public Event
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
	class HAZEL_API AppUpdateEvent:public Event
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
	class HAZEL_API AppRenderEvent :public Event
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