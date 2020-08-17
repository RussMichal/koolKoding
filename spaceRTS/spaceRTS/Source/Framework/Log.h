#pragma once
#include "spdlog\spdlog.h"
#include "spdlog\fmt\ostr.h"

#include <memory>

#include "Core.h"

namespace MYENGINE
{
	class MYENGINE_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_coreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_clientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_coreLogger;
		static std::shared_ptr<spdlog::logger> s_clientLogger;
	};
}

//core log macro
#define ME_CORE_TRACE(...)  ::MYENGINE::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define ME_CORE_INFO(...)   ::MYENGINE::Log::GetCoreLogger()->info(__VA_ARGS__)
#define ME_CORE_WARN(...)   ::MYENGINE::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define ME_CORE_ERROR(...)  ::MYENGINE::Log::GetCoreLogger()->error(__VA_ARGS__)
#define ME_CORE_FATAL(...)  ::MYENGINE::Log::GetCoreLogger()->fatal(__VA_ARGS__)

//client log macro
#define ME_TRACE(...)  ::MYENGINE::Log::GetClientLogger()->trace(__VA_ARGS__)
#define ME_INFO(...)   ::MYENGINE::Log::GetClientLogger()->info(__VA_ARGS__)
#define ME_WARN(...)   ::MYENGINE::Log::GetClientLogger()->warn(__VA_ARGS__)
#define ME_ERROR(...)  ::MYENGINE::Log::GetClientLogger()->error(__VA_ARGS__)
#define ME_FATAL(...)  ::MYENGINE::Log::GetClientLogger()->fatal(__VA_ARGS__)