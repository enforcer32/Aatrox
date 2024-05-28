#pragma once

#include "ATRXEngine/Core/Typedef.h"

namespace ATRX
{
	class ATRX_API Timer
	{
	public:
		static bool OnInit();
		static void OnDestroy();
		static double GetTimeSeconds();
		static double GetTimeMilliSeconds();
	};
}
