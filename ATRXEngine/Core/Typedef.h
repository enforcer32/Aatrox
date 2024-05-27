#pragma once

// DLL Export/Imports
#ifdef ATRX_ENGINE_BUILD
	#ifdef _MSC_VER
		#define ATRX_API __declspec(dllexport)
	#else
		#define ATRX_API __attribute__((visibility("default")))
	#endif
#else
	#ifdef _MSC_VER
		#define ATRX_API __declspec(dllimport)
	#else
		#define ATRX_API
	#endif
#endif
