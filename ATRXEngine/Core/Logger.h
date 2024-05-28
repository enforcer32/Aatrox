#pragma once

#include "ATRXEngine/Core/Typedef.h"

#include <spdlog/spdlog.h>
#include <string>
#include <memory>
#include <vector>
#include <sstream>

namespace ATRX
{
	class ATRX_API Logger
	{
	public:
		static void OnInit(const std::string& name = "AatroxLogger");
		static void OnDestroy();
		inline static std::shared_ptr<spdlog::logger> GetEngineLogger() { return s_EngineLogger; }
		inline static std::shared_ptr<spdlog::logger> GetLogger() { return s_Logger; }

		static void SaveLastLog();
		static void Clear();
		static const std::vector<std::string>& GetLogs();

	private:
		inline static std::shared_ptr<spdlog::logger> s_EngineLogger;
		inline static std::shared_ptr<spdlog::logger> s_Logger;
		inline static std::vector<std::string> s_Logs;
		inline static std::ostringstream s_Stream;
	};
}

#ifdef ATRX_ENGINE_BUILD
	#define ATRX_LOG_INFO(...)		(ATRX::Logger::GetEngineLogger()->info(__VA_ARGS__), ATRX::Logger::SaveLastLog())
	#define ATRX_LOG_WARN(...)		(ATRX::Logger::GetEngineLogger()->warn(__VA_ARGS__), ATRX::Logger::SaveLastLog())
	#ifdef ATRX_ENGINE_DEBUG
		#define ATRX_LOG_DEBUG(...)	(ATRX::Logger::GetEngineLogger()->debug(__VA_ARGS__), ATRX::Logger::SaveLastLog())
	#else
		#define ATRX_LOG_DEBUG(...)
	#endif
	#define ATRX_LOG_ERROR(...)		(ATRX::Logger::GetEngineLogger()->error(__VA_ARGS__), ATRX::Logger::SaveLastLog())
	#define ATRX_LOG_CRITICAL(...)	(ATRX::Logger::GetEngineLogger()->critical(__VA_ARGS__), ATRX::Logger::SaveLastLog(), throw)
	#define ATRX_LOG_TRACE(...)		(ATRX::Logger::GetEngineLogger()->trace(__VA_ARGS__), ATRX::Logger::SaveLastLog())
#else
	#define ATRX_LOG_INFO(...)		(ATRX::Logger::GetLogger()->info(__VA_ARGS__), ATRX::Logger::SaveLastLog())
	#define ATRX_LOG_WARN(...)		(ATRX::Logger::GetLogger()->warn(__VA_ARGS__), ATRX::Logger::SaveLastLog())
	#ifdef ATRX_DEBUG
		#define ATRX_LOG_DEBUG(...)		(ATRX::Logger::GetLogger()->debug(__VA_ARGS__), ATRX::Logger::SaveLastLog())
	#else
		#define ATRX_LOG_DEBUG(...)
	#endif
	#define ATRX_LOG_ERROR(...)		(ATRX::Logger::GetLogger()->error(__VA_ARGS__), ATRX::Logger::SaveLastLog())
	#define ATRX_LOG_CRITICAL(...)	(ATRX::Logger::GetLogger()->critical(__VA_ARGS__), ATRX::Logger::SaveLastLog(), throw)
	#define ATRX_LOG_TRACE(...)		(ATRX::Logger::GetLogger()->trace(__VA_ARGS__), ATRX::Logger::SaveLastLog(), __VA_ARGS__))
#endif
