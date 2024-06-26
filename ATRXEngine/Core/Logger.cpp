#include "ATRXEngine/Core/Logger.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/ostream_sink.h>
#include <mutex>

namespace ATRX
{
	void Logger::OnInit(const std::string& name)
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_EngineLogger = spdlog::stdout_color_mt("ATRXEngineLogger");
		s_EngineLogger->set_level(spdlog::level::trace);
		s_Logger = spdlog::stdout_color_mt(name);
		s_Logger->set_level(spdlog::level::trace);

		/*std::vector<spdlog::sink_ptr>& sinks{ s_EngineLogger->sinks() };
		sinks[0] = std::make_shared<spdlog::sinks::ostream_sink_st>(s_Stream);
		std::vector<spdlog::sink_ptr>& sinks2{ s_Logger->sinks() };
		sinks2[0] = std::make_shared<spdlog::sinks::ostream_sink_st>(s_Stream);*/
		ATRX_LOG_INFO("ATRXLogger->Initialized!");
	}

	void Logger::OnDestroy()
	{
		ATRX_LOG_INFO("ATRXLogger->Destroyed!");
	}

	void Logger::SaveLastLog()
	{
		s_Logs.push_back(s_Stream.str());
		s_Stream.str("");
	}

	void Logger::Clear()
	{
		s_Logs.clear();
	}

	const std::vector<std::string>& Logger::GetLogs()
	{
		return s_Logs;
	}
}
