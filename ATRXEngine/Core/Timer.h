#pragma once

#include "ATRXEngine/Core/Typedef.h"

namespace ATRX
{
	class ATRX_API Timer
	{
	public:
		static bool OnInit();
		static double GetTimeSeconds();
		static double GetTimeMilliSeconds();
	};
}
