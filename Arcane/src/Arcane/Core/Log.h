#pragma once

#include "Arcane/Core/Base.h"

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace Arcane
{

	class ARC_API Log
	{
	public:
		static void Init();

		inline static Shared<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static Shared<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:

		static Shared<spdlog::logger> s_CoreLogger;
		static Shared<spdlog::logger> s_ClientLogger;
	};

}


#define ARC_CORE_TRACE(...)	   ::Arcane::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define ARC_CORE_INFO(...)	   ::Arcane::Log::GetCoreLogger()->info(__VA_ARGS__)
#define ARC_CORE_WARN(...)	   ::Arcane::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define ARC_CORE_ERROR(...)	   ::Arcane::Log::GetCoreLogger()->error(__VA_ARGS__)
#define ARC_CORE_CRITICAL(...) ::Arcane::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define ARC_TRACE(...)	 ::Arcane::Log::GetClientLogger()->trace(__VA_ARGS__)
#define ARC_INFO(...)	 ::Arcane::Log::GetClientLogger()->info(__VA_ARGS__)
#define ARC_WARN(...)     ::Arcane::Log::GetClientLogger()->warn(__VA_ARGS__)
#define ARC_ERROR(...)    ::Arcane::Log::GetClientLogger()->error(__VA_ARGS__)
#define ARC_CRITICAL(...) ::Arcane::Log::GetClientLogger()->critical(__VA_ARGS__)