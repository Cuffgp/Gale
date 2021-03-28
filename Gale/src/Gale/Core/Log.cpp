#include "glpch.h"
#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace Gale {

	Ref<spdlog::logger> Log::m_Logger;

	void Log::Init()
	{
		auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		sink->set_pattern("%^[%T][%n]: %v%$");

		m_Logger = std::make_shared<spdlog::logger>("Gale", sink);
		m_Logger->set_level(spdlog::level::trace);
	}

}



