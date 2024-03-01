#pragma once

#include "Gale/Core/Base.h"

#include "spdlog/spdlog.h"

namespace Gale {

	class Log
	{
	public:
		static void Init();

		static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
	private:
		static Ref<spdlog::logger> s_CoreLogger;
	};

}

// Core log macros
#define GL_TRACE(...)    ::Gale::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define GL_INFO(...)     ::Gale::Log::GetCoreLogger()->info(__VA_ARGS__)
#define GL_WARN(...)     ::Gale::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define GL_ERROR(...)    ::Gale::Log::GetCoreLogger()->error(__VA_ARGS__)
#define GL_CRITICAL(...) ::Gale::Log::GetCoreLogger()->critical(__VA_ARGS__)

