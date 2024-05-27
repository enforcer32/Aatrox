#include <iostream>

#include <ATRXEngine/Core/Logger.h>
#include <ATRXEngine/Core/Assertion.h>

int main()
{
	ATRX::Logger::Init();

	ATRX_LOG_INFO("INFO");

	ATRX_ASSERT(1 == 0, "ERROR");

	return 0;
}
