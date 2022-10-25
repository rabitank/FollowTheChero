#pragma once


namespace Hazel
{
	class Timestep
	{
	public:
		Timestep(float time = 0.0f)
			:m_Time(time)
		{
		}
		inline float GetSeconds() { return m_Time; };
		inline float GetMilliSeconds() { return m_Time * 1000.0f; }; //毫秒

		operator float() const //use as float,添加隐式转换能力
		{
			return m_Time;
		}
	private:
		float m_Time = 0.0f;

	};

}

