#pragma once
#include "Base.h"
#include "spdlog/spdlog.h"

namespace Gale {

	class Log
	{
	public:
		static void Init();
		static std::shared_ptr<spdlog::logger>& Get() { return m_Logger; }
	private:
		static std::shared_ptr<spdlog::logger> m_Logger;
	};

}

#define GL_TRACE(...)	::Gale::Log::Get()->trace(__VA_ARGS__)
#define GL_INFO(...)	::Gale::Log::Get()->info(__VA_ARGS__)
#define GL_WARN(...)	::Gale::Log::Get()->warn(__VA_ARGS__)
#define GL_ERROR(...)	::Gale::Log::Get()->error(__VA_ARGS__)
#define GL_FATAL(...)	::Gale::Log::Get()->critical(__VA_ARGS__)
