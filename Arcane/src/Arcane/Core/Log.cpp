#include <arcpch.h>

#include "Arcane/Core/Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace Arcane
{

	Shared<spdlog::logger> Log::s_CoreLogger;
	Shared<spdlog::logger> Log::s_ClientLogger;

	void Log::Init()
	{
		std::vector<spdlog::sink_ptr> logSinks;
		logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		logSinks[0]->set_pattern("%^[%T] %n: %v%$");

#ifdef ARC_LOG_FILE
		logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Arcane.log", true));
		logSinks[1]->set_pattern("%^[%T] [%l] %n: %v%$");
#endif

		s_CoreLogger = CreateShared<spdlog::logger>("ARCANE", begin(logSinks), end(logSinks));
		s_CoreLogger->set_level(spdlog::level::trace);
		s_CoreLogger->flush_on(spdlog::level::trace);

		s_ClientLogger = CreateShared<spdlog::logger>("APP", begin(logSinks), end(logSinks));
		s_ClientLogger->set_level(spdlog::level::trace);
		s_ClientLogger->flush_on(spdlog::level::trace);
	}

}