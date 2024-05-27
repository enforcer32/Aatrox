#pragma once

#include "ATRXEngine/Core/Platform.h"
#include "ATRXEngine/Core/Logger.h"

#if defined ATRX_DEBUG || ATRX_ENGINE_DEBUG
	#ifdef ATRX_PLATFORM_WINDOWS
		#define ATRX_DEBUGBREAK() __debugbreak()	
	#else
		#error "Platform doesnt support debugbreak"
	#endif
	#define ATRX_ENABLE_ASSERTS
#else
	#define ATRX_DEBUGBREAK()
#endif

#ifdef ATRX_ENABLE_ASSERTS
	#define ATRX_ASSERT(x, msg) { if(!(x)) { ATRX_LOG_ERROR("Assertion Error: {}, File: {}, Line: {}", msg, __FILE__, __LINE__); ATRX_DEBUGBREAK(); } }
#else
	#define ATRX_ASSERT(...)
#endif
