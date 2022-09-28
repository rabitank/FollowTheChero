#include "Log.h"


namespace Hazel
{

	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

 	void Log::Init()
	{

		auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		console_sink->set_level(spdlog::level::trace); //要高于trace 0.0才输出
		console_sink->set_pattern("%^[%T] %n: %v%$");

		s_CoreLogger= std::make_shared<spdlog::logger>(spdlog::logger("HAZEL", console_sink));
		s_ClientLogger = std::make_shared<spdlog::logger>(spdlog::logger("APP", console_sink));
	}


}