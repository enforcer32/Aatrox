#include "ATRXEngine/Core/Timer.h"
#include "ATRXEngine/Core/Logger.h"

#include <cstdint>
#include <Windows.h>

namespace ATRX
{
	static double clockFreq;
	static LARGE_INTEGER startTime;

	bool Timer::OnInit()
	{
		LARGE_INTEGER freq;
		QueryPerformanceFrequency(&freq);
		clockFreq = 1.0 / (double)freq.QuadPart;
		QueryPerformanceCounter(&startTime);
		ATRX_LOG_INFO("ATRXTimer->Initialized!");
		return true;
	}

	void Timer::OnDestroy()
	{
		ATRX_LOG_INFO("ATRXTimer->Destroyed!");
	}

	double Timer::GetTimeSeconds()
	{
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		return (double)now.QuadPart * clockFreq;
	}

	double Timer::GetTimeMilliSeconds()
	{
		return GetTimeSeconds() * 1000;
	}
}
